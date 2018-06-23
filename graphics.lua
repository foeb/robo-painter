local gd = require "gd"

local cb = require "cranberry"
local liblang = require "liblang"

function getName(seed, maxdepth, width, height)
  return "exp-" .. seed .. "_" .. maxdepth .. "x" .. width .. "x" .. height
end

function expToString(exp, seed, maxdepth, width, height)
  result = "return {\n" ..
    "  seed = " .. seed  .. ",\n" ..
    "  maxdepth = " .. maxdepth .. ",\n" ..
    "  width = " .. width .. ",\n" ..
    "  height = " .. height .. ",\n" ..
    "  exp = {\n    "
  for i = 1, liblang.length(exp) do
    if (i-1) % 16 == 0 and i ~= 1 then result = result .. '\n    ' end
    result = result .. string.format('0x%02x, ', liblang.geti(exp, i))
  end
  result = result .. '\n  }\n}\n'
  return result
end

function generateAndSave(seed, maxdepth, width, height, dir)
  local im = gd.createTrueColor(width, height)
  dir = dir or ""

  local exp
  if type(seed) == "table" then
    exp = liblang.to_exp(seed)
    seed = 0
  else
    print("\tgenerating " .. seed)
    exp = liblang.generate(seed, maxdepth)
  end

  print("\twriting "  ..  seed)
  local txt = io.open(dir .. getName(seed, maxdepth, width, height)..".lua", "w+")
  print(expToString(exp, seed, maxdepth, width, height))
  txt:write(expToString(exp, seed, maxdepth, width, height))
  txt:close()

  print("\tinterpreting " .. seed)
  local expmap = {}
  io.write("\t\t")
  for y = 1, height do
    if y % math.ceil(height/16) == 0 then
      io.write(".")
      io.flush()
    end
    for x = 1, width do
      expmap[(y-1)*width + x] = liblang.interpret(exp, x, y)
    end
  end
  print("")

  -- Fit the map to a [0,1] range
  print("\tfitting " .. seed)
  local max = cb.max(expmap)
  local min = cb.min(expmap)
  if max ~= min then
    cb.map_(function(x) return (x - min)/(max - min) end, expmap)
  end

  function getColor(x)
    function toRadians(y)
      return y * math.pi
    end

    function value(offset)
      return math.floor((math.floor(math.cos(toRadians(x) + offset) ^ 2 * 255) % 256) * (1 - (1-x)^5))
    end

    return im:colorExact(
        value(0.33 * 2 * math.pi/3), value(0), value(0.66 * 4 * math.pi/3))
  end

  print("\tdrawing " .. seed)
  for y = 1, height do
    for x = 1, width do
      im:setPixel(x-1, y-1, getColor(expmap[(y-1)*width + x]))
    end
  end

  print("\tsaving " .. seed)
  im:png(dir .. getName(seed, maxdepth, width, height)..".png")
  print("\tdone!")
end

local seed = arg[1]
local maxdepth = arg[2] or 7
local width = arg[3] or 100
local height = arg[4] or 100
local dir = arg[5] or ""


if seed then
  generateAndSave(seed, maxdepth, width, height, dir)
else
  for i = 1, 100 do
    print(tostring(i/100))
    generateAndSave(i + 1100, 5, 512, 512, "images/")
  end
end
--generateAndSave({}, 0, 512, 512)


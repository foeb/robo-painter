package.path = package.path .. ";lib/cranberry/?.lua;lib/serpent/src/?.lua"
package.cpath = package.cpath .. ";lib/lua-gd/?.so;?.so"

local gd = require "gd"
local cb = require "cranberry"
local liblang = require "liblang"

local function getName(seed, maxdepth, width, height)
  return "exp-" .. seed .. "_" .. maxdepth .. "x" .. width .. "x" .. height
end

local function expToString(exp, seed, maxdepth, width, height)
  local result = "return {\n" ..
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

local function getColor(im, x, rOffset, gOffset, bOffset)
  local function toRadians(y)
    return y * math.pi
  end

  local function toValid(y)
    if y < 0 then
      return 0
    elseif y > 255 then
      return 255
    else
      return y
    end
  end

  local function value(offset)
    return toValid(math.floor((math.floor(math.cos(toRadians(x) + offset) ^ 2 * 255) % 255) * (1 - (1-x)^5)))
  end

  return im:colorExact(
      value(rOffset), value(gOffset), value(bOffset))
end

local function generateAndSave(seed, maxdepth, width, height, dir, rOffset, gOffset, bOffset)
  local maxdepth = maxdepth or 7
  local width = width or 128
  local height = height or 128
  local dir = dir or "images/"
  local rOffset = rOffset or 0.33 * 2 * math.pi/3
  local gOffset = gOffset or 0
  local bOffset = bOffset or 0.66 * 4 * math.pi/3

  local im = gd.createTrueColor(width, height)

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


  print("\tdrawing " .. seed)
  for y = 1, height do
    for x = 1, width do
      im:setPixel(x-1, y-1, getColor(im, expmap[(y-1)*width + x], rOffset, gOffset, bOffset))
    end
  end

  print("\tsaving " .. seed)
  im:png(dir .. getName(seed, maxdepth, width, height)..".png")
  print("\tdone!")
end

math.randomseed(os.time())

local iterations = arg[1] or 10
local seed       = arg[2] or math.random(2147483647)
local maxdepth   = arg[3]
local width      = arg[4]
local height     = arg[5]
local dir        = arg[6]
local rOffset    = arg[7]
local gOffset    = arg[8]
local bOffset    = arg[9]

for i = 0, iterations - 1 do
  print(tostring(math.floor(100 * i/iterations)) .. "%")
  generateAndSave(i + seed, maxdepth, width, height, dir, rOffset, gOffset, bOffset)
end

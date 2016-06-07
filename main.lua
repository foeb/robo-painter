local perlin = require "perlin"
local language = require "language"
local genetic = require "genetic"
local utility = require "utility"
local loveterm = require "lib/loveterm/loveterm"
local serpent = require "lib/serpent/src/serpent"
local cb = require "lib/cranberry/cranberry"

function love.load()
  screen = loveterm.create(
      "lib/loveterm/tilesets/CGA8x8thin.png",
      10, 10)
--    love.graphics.getWidth()/8,
--    love.graphics.getHeight()/8)
  seed = 100
  maxdepth = 40
  exp = language.generateTree(maxdepth, seed)
  imgdata = love.image.newImageData(101, 101)
  img = nil
  render(seed)
end

function love.draw()
  screen:draw()
  --love.graphics.draw(img)
end

function love.keypressed(key)
  if key == "space" then
    render()
  elseif key == "left" then
    seed = seed - 1
    render(seed)
  elseif key == "right" then
    seed = seed + 1
    render(seed)
  elseif key == "," then
    maxdepth = maxdepth - 1
    render(maxdepth)
  elseif key == "." then
    maxdepth = maxdepth + 1
    render(maxdepth)
  end
end

function render(seed)
  print("generating...")
  local exp = language.generateTree(maxdepth, seed)
  print("done!")
  if seed == 100 then  exp = { "x" } end
  local expmap = {}
  for y = 0, screen.height - 1 do
    for x = 0, screen.width - 1 do
      io.write(".")
      expmap[x + y * screen.width] = language.interpret(exp, x, y)
    end
    print("")
  end
  local max = cb.max(expmap)
  local min = cb.min(expmap)
  cb.map_(function(x) return ((x - min) / (max == 0 and 1 or max)) * 2 * math.pi end, expmap)
  i = 0
  screen:clear()
  cb.each(function(x) screen:setbg({ math.sin(x)*255, math.sin(x + 0.2 * math.pi/3)*255, x*255 }, i); i = i + 1 end, expmap)
  
      --[[
  for y = 0, screen.height - 1 do
    for x = 0, screen.width - 1 do
      --local shade = (0.3 + 0.11*language.interpret(exp, x, y)) * 255
      if utility.isnan(shade) or utility.isnan(shade) then
        shade = 0
      end
      screen:setbg({ shade, shade, shade, 255 }, x, y)
      --print(screen.bg[x + y * screen.width][1])
    end
  end
      --]]
  screen:print(tostring(maxdepth) .. ":" .. tostring(seed))
  --[[
  for y = 1, 100 do
    for x = 1, 100 do
      local shade = (0.3 + 0.11*language.interpret(exp, x, y)) * 255
      imgdata:setPixel(x, y, 255, 255, 255, shade)
    end
  end
  img = love.graphics.newImage(imgdata)]]
end

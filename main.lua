local perlin = require "perlin"
local loveterm = require "lib/loveterm/loveterm"

function love.load()
  screen = loveterm.create(
      "lib/loveterm/tilesets/CGA8x8thin.png",
      love.graphics.getWidth()/8,
      love.graphics.getHeight()/8)
  noise = perlin.create()
  for y = 0, screen.height - 1 do
    for x = 0, screen.width - 1 do
      local shade = (0.3 + 1.2*noise:generate(x/9, y/9)) * 255
      screen:setbg({ shade, shade, shade }, x, y)
    end
  end
end

function love.draw()
  screen:draw()
end


local cb = require "lib/cranberry/cranberry"

local perlin = {}

local function lerp(n, m, w)
  return (1 - w) * n + w * m
end

local function generateGradientGrid(size)
  local result = {}
  for i = 0, size - 1 do
    local angle = math.random(0, 2 * math.pi)
    result[i] = { math.cos(angle), math.sin(angle) }
  end
  --[[ 
  for i = 0, width * height - 1 do
    local angle = math.random(0, 2 * math.pi)
    result[i] = { math.cos(angle), math.sin(angle) }
  end]]
  return result
end

local function generatePermtable(size)
  local t = {}
  for i = 1, size do
    t[i] = i
  end
  cb.shuffle_(t)
  t[0] = t[size]
  return t
end

local function get(permtable, grid, size, x, y)
  -- return grid[x + y * grid.width]
  return grid[(x + permtable[y]) % size]
end

local function dotGrid(permtable, grid, size, gx, gy, x, y)
  local dx = x - gx
  local dy = y - gy
  local point = get(permtable, grid, size, gx, gy) or { 0, 0 }
  return dx * point[1] + dy * point[2]
end

function perlin.create(size)
  local p = {}
  p.size = size or 256
  p.grid = generateGradientGrid(p.size)
  p.permtable = generatePermtable(p.size)
  setmetatable(p, { __index = perlin })
  return p
end

function perlin:generate(x, y)
  local x1 = math.floor(x)
  local x2 = x1 + 1
  local y1 = math.floor(y)
  local y2 = y1 + 1
  local function weight(p)
    return 3 * p^2 - 2 * p^3
  end
  local sx = weight(x - x1)
  local sy = weight(y - y1)
  local n1 = dotGrid(self.permtable, self.grid, self.size, x1, y1, x, y)
  local n2 = dotGrid(self.permtable, self.grid, self.size, x2, y1, x, y)
  local ix1 = lerp(n1, n2, sx)
  n1 = dotGrid(self.permtable, self.grid, self.size, x1, y2, x, y)
  n2 = dotGrid(self.permtable, self.grid, self.size, x2, y2, x, y)
  local ix2 = lerp(n1, n2, sx)
  return lerp(ix1, ix2, sy)
end

return perlin


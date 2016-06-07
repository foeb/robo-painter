local utility = require "utility"
local cb = require "lib/cranberry/cranberry"

local perlin = {}

local function lerp(n, m, w)
  return (1 - w) * n + w * m
end

local function generateGradientGrid(size)
  local result = {}
  for i = 0, size - 1 do
    local angle = math.random() * math.pi * 2
    result[i] = { math.cos(angle), math.sin(angle) }
  end
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

function perlin:get(x, y)
  x = ((utility.isnan(x) or utility.isinf(x) or x == nil) and 0) or x
  y = ((utility.isnan(y) or utility.isinf(y) or y == nil) and 0) or y
  return self.grid[(x + self.permtable[y % self.size]) % self.size]
end

function perlin:dotGrid(gx, gy, x, y)
  local dx = x - gx
  local dy = y - gy
  local point = self:get(gx, gy) or { 0, 0 }
  return dx * point[1] + dy * point[2]
end

function perlin.create(size)
  local self = setmetatable({}, { __index = perlin })
  self.size = size or 256
  self.grid = generateGradientGrid(self.size)
  self.permtable = generatePermtable(self.size)
  return self
end

function perlin:generate(x, y)
  local x1 = math.floor(x)
  local x2 = x1 + 1
  local y1 = math.floor(y)
  local y2 = y1 + 1
  local function weightHermite(p) -- Perlin recommends against this one
    return 3 * p^2 - 2 * p^3
  end
  local function weight(p) -- has a continuous second derivative everywhere
    return 6 * p^5 - 15 * p^4 + 10 * p^3
  end
  local sx = weight(x - x1)
  local sy = weight(y - y1)
  local n1 = self:dotGrid(x1, y1, x, y)
  local n2 = self:dotGrid(x2, y1, x, y)
  local ix1 = lerp(n1, n2, sx)
  n1 = self:dotGrid(x1, y2, x, y)
  n2 = self:dotGrid(x2, y2, x, y)
  local ix2 = lerp(n1, n2, sx)
  return lerp(ix1, ix2, sy)
end

return perlin


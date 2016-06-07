
local perlin = require "perlin"
local utility = require "utility"

local language = {}

local noise = perlin.create()

local function assert2(a, b)
  assert(a ~= nil and b ~= nil)
end

language.operations = {
  add = {
    fun = function(x, y, a1, a2) 
      assert2(a1, a2)
      return a1 + a2
    end,
    args = 2
  },
  sub = {
    fun = function(x, y, a1, a2) 
      assert2(a1, a2)
      return a1 - a2 
    end,
    args = 2
  },
  mul = {
    fun = function(x, y, a1, a2)
      assert2(a1, a2)
      return a1 * a2
    end,
    args = 2
  },
  div = {
    fun = function(x, y, a1, a2)
      assert2(a1, a2)
      if a2 == 0 then
        return a1
      end
      return a1 / a2
    end,
    args = 2
  },
  pow = {
    fun = function(x, y, a1, a2)
      assert2(a1, a2)
      return a1 ^ a2
    end,
    args = 2
  },
  sin = {
    fun = function(x, y, a)
      assert(a ~= nil)
      return math.sin(a)
    end,
    args = 1
  },
  cos = {
    fun = function(x, y, a)
      assert(a ~= nil)
      return math.cos(a)
    end,
    args = 1
  },
  tan = {
    fun = function(x, y, a)
      assert(a ~= nil)
      return math.tan(a)
    end,
    args = 1
  },
  abs = {
    fun = function(x, y, a)
      assert(a ~= nil)
      return math.abs(a)
    end,
    args = 1
  },
  exp = {
    fun = function(x, y, a)
      assert(a ~= nil)
      return math.exp(a)
    end,
    args = 1
  },
  log = {
    fun = function(x, y, a)
      assert(a ~= nil)
      return math.log(a)
    end,
    args = 1
  },
  min = {
    fun = function(x, y, a1, a2)
      assert2(a1, a2)
      return math.min(a1, a2)
    end,
    args = 2
  },
  max = {
    fun = function(x, y, a1, a2)
      assert2(a1, a2)
      return math.max(a1, a2)
    end,
    args = 2
  },
  sqrt = {
    fun = function(x, y, a)
      assert(a ~= nil)
      return math.sqrt(a)
    end,
    args = 1
  },
  random = {
    fun = function(x, y)
      return math.random()
    end,
    args = 0
  },
  x = {
    fun = function(x, y) return x end,
    args = 0
  },
  y = {
    fun = function(x, y) return y end,
    args = 0
  },
  perlin = {
    fun = function(x, y, a)
      a = a or math.pi/2
      return noise:generate(x * a, y * a)
    end,
    args = 1
  },
}

language.terminal = { "x", "y", "random", "perlin" }

function language.getOperations()
  local result = {}
  local i = 1
  for k,_ in pairs(language.operations) do
    result[i] = k
    i = i + 1
  end
  return result
end

function language.interpret(tree, x, y)
  if type(tree) == "table" then
    local ops = tree[1]
    assert(type(ops) == "string", "tree[1] must be a string")
    local op = language.operations[ops]
    assert(op ~= nil, "Operation not found: " .. ops)
    local a1 = language.interpret(tree[2], x, y)
    local a2 = language.interpret(tree[3], x, y)
    local result = op.fun(x, y, a1, a2)
    if utility.isinf(result) or utility.isnan(result) then
      result = math.random()
    end
    return result
  else
    return tree
  end
end

function language.generateTree(maxdepth, seed)
  seed = seed or os.time()
  math.randomseed(seed)
  local operations = language.getOperations()
  local function recurse(depth)
    if depth >= maxdepth then
      return { language.terminal[math.random(#language.terminal)] }
    else
      local ops = operations[math.random(#operations)]
      local t = { ops }
      for i = 1, language.operations[ops].args do
        t[i + 1] = recurse(depth + 1)
      end
      return t
    end
  end
  return recurse(1)
end

return language


local perlin = require "perlin"

local language = {}

local function assert2(a, b)
  assert(a ~= nil and b ~= nil)
end

language.operations = {
  add = function(x, y, a1, a2) 
    assert2(a1, a2)
    return a1 + a2 
  end,
  sub = function(x, y, a1, a2) 
    assert2(a1, a2)
    return a1 - a2 
  end,
  sin = function(x, y, a)
    assert(a ~= nil)
    return math.sin(a)
  end
}

function language.interpret(tree, x, y)
  if type(tree) == "table" then
    local ops = tree[1]
    assert(type(ops) == "string", "tree[1] must be a string")
    local op = language.operations[ops]
    assert(op ~= nil, "Operation not found: " .. ops)
    return op(
        x, y,
        language.interpret(tree[2], x, y),
        language.interpret(tree[3], x, y))
  else
    return tree
  end
end

return language

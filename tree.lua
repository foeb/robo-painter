local tree = {}

function tree.create(v)
  self = setmetatable({}, { __index = tree })
  self.v = v or {}
  return self
end

--- Return a function that performs a depth-first search on this tree using 
--`fun`.
--
-- Returns `acc`, `newTree` in that order.
-- @tparam fun function(t,i,acc) returns the next accumulator value based on
-- `acc` and a new tree node `newTree` to put at `i` in a new tree or does nothing if
-- it is nil.
-- @param init the initial accumulator value to plug into `fun` as `acc`
function tree.dfs(fun, init)
  return function(self)
    local function recurse(t, i, acc)
      -- nil trees have zero length
      if t == nil then
        return acc, nil, i
      end
      -- we found another node, increment our counter and get our new acc
      i = i + 1
      local newTree
      acc, newTree = fun(t, i, acc)
      -- always return t as the new tree at the first step
      newTree = newTree or (i == 1 and t) 
      -- begin our search of subtrees
      if type(t) == "table" then
        local newTreeNode
        for j = 2, #t do
          acc, newTreeNode, i = recurse(t[j], i, acc)
          if type(newTree) == "table" and newTreeNode then 
            newTree[j] = newTreeNode 
          end
        end
      end

      return acc, newTree, i
    end
    return recurse(self.v, 0, init)
  end
end

tree.getTotalNodes = tree.dfs(function(_, i, _)
  return i
end, 0)

function tree:getRandomNode()
  local length = self:getTotalNodes()
  if length > 0 then
    return math.random(length)
  end
end

--- Get the nth node of this tree
function tree:getAt(n) 
  return (tree.dfs(function(t, i, acc)
    if i == n then
      return t
    else
      return acc
    end
  end, nil))(self)
end

function tree:setAt(n, v)
  local _, newTree = (tree.dfs(function(t, i, found)
    if i == n then
      return i == n, v
    end
    return found, nil
  end, false))(self)
  return newTree
end

return tree

local cb = require "lib/cranberry/cranberry"

local genetic = {}

local function chooseAtRandom(max, n)
  local a = {}
  for i = 1, max do
    a[i] = i
  end
  cb.shuffle_(a)
  return cb.take(n, a)
end

function genetic.tournament(size, p, fitnessfn, population)
  local choseni = chooseAtRandom(#population, size)
  local chosen = cb.map(
    function(i) 
      return { fitness = fitnessfn(population[i]), chromosome = population[i] }
    end, 
    choseni)
  -- sort descending by fitness:
  cb.sort_(chosen, function(a, b) return a.fitness > b.fitness end)
  local i = 1
  local r
  while i <= #chosen do
    r = math.random()
    if r <= p * (1 - p) ^ (i - 1) then
      return chosen[i].chromosome
    end
  end
  -- I don't know if this is correct, but I don't want any nils popping up
  return chosen[1]
end

-- FIXME
function genetic.splice(parent1, parent2, p)
  p = p or 0.5

  -- TODO use DFS and return a random int instead of an index list
  -- i.e. flatten the tree, pick a random element and keep the index for later
  local function getRandom(root)
    print("starting random pick")
    local current = root
    local loc = {}
    local i = 1
    while true do
      if type(current) ~= "table" then
        break
      end
      local rnext = math.random(#current - 1)
      if rnext == 0 then break end
      loc[i] = rnext + 1
      print("i:" .. i)
      if math.random() <= p then 
        break 
      end
      i = i + 1
      current = current[rnext]
    end
    return loc
  end

  local function getAt(a, n, ...)
    if n then
      return getAt(a[n], ...)
    end
    return a
  end

  local function replaceAt(tree, loc, subtree)
    if type(tree) ~= "table" then
      return tree
    end
    if #loc == 0 then
      return subtree
    end
    local result = {}
    for i = 1, #tree do
      if i == loc[1] then
        result[i] = replaceAt(tree[i], cb.tail(loc), subtree)
      else
        result[i] = tree[i]
      end
    end
    return result
  end

  local loc1 = getRandom(parent1)
  local loc2 = getRandom(parent2)
  local subtree1 = getAt(parent1, table.unpack(loc1))
  local subtree2 = getAt(parent2, table.unpack(loc2))
  return replaceAt(parent1, loc1, subtree1), replaceAt(parent2, loc2, subtree2)
end

return genetic

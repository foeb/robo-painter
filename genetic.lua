local tree = require "tree"
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

function genetic.splice(parent1, parent2)
  local t1 = tree.create(parent1)
  local t2 = tree.create(parent2)

  local loc1 = t1:getRandomNode()
  local loc2 = t2:getRandomNode()
  local subtree1 = t1:getAt(loc1)
  local subtree2 = t2:getAt(loc2)
  return t1:setAt(loc1, subtree2), t2:setAt(loc2, subtree1)
end

return genetic

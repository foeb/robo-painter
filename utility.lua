local utility = {}

function utility.isnan(x)
  -- FIXME may not be portable
  return x ~= x
end

function utility.isinf(x)
  return x == math.huge or x == -math.huge
end

return utility

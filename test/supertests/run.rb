tests =  Dir["in/*.rb"]

tests.each do |test|
  system("ruby #{test} > #{test.delete('in/')}.out")
end

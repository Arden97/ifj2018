tests =  Dir["in/*.rb"]

tests.each do |test|
  system("ruby #{test} > out/#{test.slice(3..-1).chomp('.rb')}.out")
end

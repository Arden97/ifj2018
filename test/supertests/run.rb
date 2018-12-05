tests =  Dir["in/*.rb"]
tests.each do |test|
  puts "{     
          \"name\": \"#{test.slice(3..-1).chomp('.rb')}\",
          \"input\": \"#{test}\",
          \"output\": \"out/#{test.slice(3..-1).chomp('.rb')}.out\",
          \"type\": \"file\",
          \"expected\": \"success\"
        },"
end


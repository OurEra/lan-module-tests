
# set target generate class name
generate_class = "TestDeque"

# templete file
templete_header = open("cpp_header_templete.txt", "r",encoding='utf-8')
templete_source = open("cpp_source_templete.txt", "r",encoding='utf-8')

# output file
out_header = open(generate_class + ".h", "w",encoding='utf-8')
out_source = open(generate_class + ".cpp", "w",encoding='utf-8')

for line in templete_header:
    out_header.write(line.replace('TestTemplete', generate_class))
for line in templete_source:
    out_source.write(line.replace('TestTemplete', generate_class))

templete_header.close()
templete_source.close()
out_header.close()
out_source.close()

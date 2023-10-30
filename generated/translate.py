import os

folder = "programs"
output = "code"

wrapper="""
#if defined(__clang__)
extern "C" void {name}(int* buffer) {{
  asm volatile(
    #include "{clang_path}"
    : "+r"(buffer)
    :
    : "eax", "ecx", "edx", "r8d", "memory");
}}
#else
#endif
"""

functions = []
wrappers = []
for score in sorted(os.listdir(folder), key=lambda x: int(x)):
    score_folder = os.path.join(folder, score)
    wrapper_file = os.path.join(output, score, "wrapper.h")
    wrappers.append(os.path.join(score, "wrapper.h"))
    wrapper_code = ""
    for criticalpath in sorted(os.listdir(score_folder), key=lambda x: int(x)):
        program_folder = os.path.join(score_folder, criticalpath)
        
        numbers = os.listdir(program_folder)
        numbers = [int(n.split(".")[0]) for n in numbers if n.endswith(".txt")]
        numbers = sorted(numbers)
        numbers = [str(n) for n in numbers]
        for number in numbers:

            file = os.path.join(folder, score, criticalpath, number+".txt")

            # file = "programs/32/7/1.txt"
            with open(file, "r") as f:
                lines = f.readlines()

            program = []
            for line in lines:
                line = line.strip()
                if line == "":
                    continue
                instr, a1, a2 = line.split(" ")
                program.append((instr, int(a1), int(a2)))
                
            reg_mapping = {
                1: "%%r8d",
                2: "%%eax",
                3: "%%ecx",
                4: "%%edx",
            }

                
            code = []
            code.append("mov (%0), " + reg_mapping[1])
            code.append("mov 0x4(%0), " + reg_mapping[2])
            code.append("mov 0x8(%0), " + reg_mapping[3])

            for instr, a, b in program:
                if instr == "cmp":
                    code.append("cmp " + reg_mapping[b] + ", " + reg_mapping[a])
                elif instr == "mov":
                    code.append("mov " + reg_mapping[b] + ", " + reg_mapping[a])
                elif instr == "cmovl":
                    code.append("cmovl " + reg_mapping[b] + ", " + reg_mapping[a])
                elif instr == "cmovg":
                    code.append("cmovg " + reg_mapping[b] + ", " + reg_mapping[a])
                    
            code.append("mov " + reg_mapping[1] + ", (%0)")
            code.append("mov " + reg_mapping[2] + ", 0x4(%0)")
            code.append("mov " + reg_mapping[3] + ", 0x8(%0)")


            output_file = os.path.join(output, score, criticalpath, number+".c")
            if not os.path.exists(os.path.dirname(output_file)):
                os.makedirs(os.path.dirname(output_file))
                
            with open(output_file, "w") as f:
                for line in code:
                    f.write("\""+line+"\t\\n\"\n")
            # for line in code:
            #     print("\t\""+line+"\t\\n\"")
            function_name = "sort3_"+score+"_"+criticalpath+"_"+number
            wrapper_code += wrapper.format(
                name=function_name, 
                clang_path=os.path.join(criticalpath, number+".c")
            )
            wrapper_code += "\n"*2
            functions.append(function_name)
            
    with open(wrapper_file, "w") as f:
        f.write(wrapper_code)
        
with open(os.path.join(output,"generated.h"), "w") as f:
    # f.write("#include \"../interface.h\"\n")
    #     f.write("""
    # #include <string>

    # using SortingFunction = void (*)(int*);

    # struct TestFunction {
    #     SortingFunction func;
    #     std::string name;
    # };
    # """.strip()+"\n")
    for wrapper_file in wrappers:
        f.write("#include \""+wrapper_file+"\"\n")
        
    f.write("TestFunction gen_functions[] = {\n")
    for function in functions:
        f.write("    {"+function+", \""+function+"\"},\n")
    f.write("};\n")
            
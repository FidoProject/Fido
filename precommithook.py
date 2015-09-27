import os

def includesDocumentation(fileContents):
	lines = fileContents.split('\n');

	classLines = [s for s in lines if s.strip().startswith("class") and s.strip().endswith(";") == False]
	if len(classLines) <= 0:
		return;

	beginingClassLine = lines.index(classLines[0])
	
	methodLines = [s for s in lines if lines.index(s) > beginingClassLine and s.strip().startswith("/*") == False and s.strip().startswith("*/") == False and s.strip().startswith("//") == False and s.strip().startswith("public") == False and len(s.replace(" ", "")) > 0]
	if len(methodLines) <= 0:
		return
	for methodLine in methodLines:
		if lines[lines.index(methodLines[0]) - 1].strip().startswith("//") == False and lines[lines.index(methodLines[0]) - 1].strip().startswith("/*") == False:
			print("No DOCUMENTATION!!!!")
			return

for root, dirs, files in os.walk("C:/Users/Michael/Documents/Fido"):
    for file in files:
        if file.endswith(".h"):
            with open(os.path.join(root, file)) as f:
            	print(file)
            	includesDocumentation(f.read());
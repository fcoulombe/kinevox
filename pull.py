

import os
import os.path


def main():
	os.chdir("lib/gcl")
	os.system("git pull")
	os.chdir("../../")
	os.system("git pull")	
if __name__ == "__main__":
	main()



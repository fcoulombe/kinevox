
import os
import os.path


def main():
	if not os.path.exists("lib/gcl"):
		os.system("git clone https://github.com/fcoulombe/gcl.git lib/gcl")
	if not os.path.exists("3rdParty"):
		os.system("git clone https://github.com/fcoulombe/kinevox_3rdparty.git 3rdParty")
		os.chdir("3rdParty")
		os.system("python clone.py")
		os.chdir("..")

if __name__ == "__main__":
	main()


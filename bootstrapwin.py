
import os
import os.path


def main():
	if not os.path.exists("lib/gcl"):
		os.system("git clone https://github.com/fcoulombe/gcl.git lib/gcl")
	os.system("python tools/Install3rdparty.py")

if __name__ == "__main__":
	main()


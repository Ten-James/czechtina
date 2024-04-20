VERSION = "0.9.1"
import os


def change_version_in_code() -> str:
    relative_path = "./src/cli/cli.cz"
    find = "version = \""
    old = ""
    with open(relative_path, "r") as file:
        lines = file.readlines()
        for line in lines:
            if find in line:
                before = line[0:line.find(find) + len(find)]
                old = line[line.find(find) + len(find):line.find("\"", line.find(find) + len(find))]
                new_version = VERSION
                if old != new_version:
                    print(f"Changing version from {old} to {new_version}")
                    new_line = line.replace(old, new_version)
                    lines[lines.index(line)] = new_line
                    break
    with open(relative_path, "w") as file:
        file.writelines(lines)
    return old

def change_version_in_readme():
    relative_path = "./README.md"
    find = "Version "
    with open(relative_path, "r") as file:
        lines = file.readlines()
        for line in lines:
            if line.startswith(find):
                old_version = line[len(find):line.find("\n", len(find))]
                new_version = VERSION
                if old_version != new_version:
                    print(f"Changing version from {old_version} to {new_version}")
                    new_line = line.replace(old_version, new_version)
                    lines[lines.index(line)] = new_line
                    break
    with open(relative_path, "w") as file:
        file.writelines(lines)  


def create_stable():
    cpCmd = "cp ./build/* ./stable/"
    os.system(cpCmd)


def create_unstable():
    cpCmd = "cp ./build/* ./unstable/"
    os.system(cpCmd)

if __name__ == "__main__":
    old = change_version_in_code()
    change_version_in_readme()
    split_old = old.split(".")
    split_new = VERSION.split(".")
    if split_old[1] != split_new[1] or split_old[0] != split_new[0]:
        create_stable()
    create_unstable()
    print("Version changed")

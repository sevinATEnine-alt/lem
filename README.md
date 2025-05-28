# *** WIP ***

# LEM

Linux Executable Manager, also know as LEM, is a powerful diverse package manager.
LEM is primarily made for Linux, but is supported by MacOS and other systems.

| System      | Supported |
| ----------- | :-------: |
| Linux       | ✅        |
| MacOS       | ✅        |
| FreeBSD     | ✅        |
| ChromeOS    | ✅        |
| Android     | ✅        |
| Windows     | ❌        |

# Table Of Contents
- [LEM](#lem)
- [Table Of Contents](#table-of-contents)
- [Install](#install)
  * [Linux](#linux)
  * [MacOS](#macos)
  * [From source](#from-source)
- [Installing LEM packages](#installing-lem-packages)
- [Creating LEM packages](#creating-lem-packages)
- [LEM commands](#lem-commands)

# Install

All installations are available in the releases section of the github repository. Alternativley, brew and apt packages are available.

## Linux

Go to the releases section and select the latest version. Then click download the Linux version.

### OR

Install with apt:
```bash
sudo apt install lem
```

## MacOS

Go to the releases section and select the latest version. Then download the MacOS version.

### OR

Install with brew:
```bash
sudo brew install lem
```

## From source

### Install libraries:

Linux:
```bash
sudo apt install libssl-dev curl libarchive-dev
```

MacOS:
```bash
brew install openssl libarchive curl 
```

### Clone the repository:

```bash
git clone https://github.com/sevinATEnine-alt/lem.git
cd lem
make
make install
```
 
# LEM commands

# Installing LEM packages

# Creating LEM packages

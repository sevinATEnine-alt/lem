# *** WIP ***

# LEM

Linux Executable Manager, also know as LEM, is a powerful diverse package manager.
LEM is primarily made for Linux, but is supported by MacOS and other systems.

Learn about how to use LEM at [lem.readthedocs.io](https://lem.readthedocs.io/en/latest/)

| System      | Supported |
| ----------- | :-------: |
| Linux       | ✅        |
| MacOS       | ✅        |
| FreeBSD     | ✅        |
| ChromeOS    | ✅        |
| Android     | ✅        |
| Windows     | ❌        |

# Table Of Contents

- [*** WIP ***](#----wip----)
- [LEM](#lem)
- [Table Of Contents](#table-of-contents)
- [Install](#install)
  * [Linux](#linux)
  * [MacOS](#macos)
  * [From source](#from-source)
    + [Install libraries](#install-libraries)
    + [Clone the repository](#clone-the-repository)
    + [Build and install](#build-and-install)
  * [Cross-compile from source](#cross-compile-from-source)
    + [Install libraries](#install-libraries-1)
    + [Install Docker Desktop](#install-docker-desktop)
    + [Clone the repository](#clone-the-repository-1)
    + [If cross compiling for MacOS](#if-cross-compiling-for-macos)
      - [Install x86_64 version of brew](#install-x86_64-version-of-brew)
      - [Install packages](#install-packages)
      - [Cross-compile](#cross-compile)
- [LEM docs](#lem-docs)

# Install

All installations are available in the releases section of the github repository. Alternativley, brew and apt packages are available.

## Linux

Go to the releases section and select the latest version. Then click download the Linux version.

### OR

**NOT WORKING YET!!!!!**

Install with apt:
```bash
sudo apt install lem
```

## MacOS

Go to the releases section and select the latest version. Then download the MacOS version.

### OR

Install with brew:

**NOT WORKING YET!!!!!**

```bash
sudo brew install lem
```

## From source

### Install libraries

Linux:

```bash
sudo apt install libssl-dev curl libarchive-dev make
```

MacOS:

```bash
brew install openssl libarchive curl make 
```

### Clone the repository

```bash

git clone https://github.com/sevinATEnine-alt/lem.git
cd lem/manager
```

### Build and install

```bash
make
make install
```


## Cross-compile from source

### Install libraries

Linux:

```bash
sudo apt install libssl-dev curl libarchive-dev make
```

MacOS:

```bash
brew install openssl libarchive curl make
```

### Install Docker Desktop

Go to [https://www.docker.com/products/docker-desktop/](https://www.docker.com/products/docker-desktop/) and download for your operating system. Then run Docker Destop to start the daemon service.

### Clone the repository

```bash

git clone https://github.com/sevinATEnine-alt/lem.git
cd lem/manager
```

### If cross compiling for MacOS
> NOTE: To cross-compile for MacOS, you must be on a arm64 proccessor running MacOS.

#### Install x86_64 version of brew

```bash
arch -x86_64 /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

#### Install packages

```bash
arch -x86_64 /usr/local/bin/brew install openssl libarchive curl make
```

#### Cross-compile

Make sure Docker is running.

```bash
./cross-compile.sh
```

# LEM docs

To learn more about lem and it's commands, go to [lem.readthedocs.io](https://lem.readthedocs.io/en/latest/).

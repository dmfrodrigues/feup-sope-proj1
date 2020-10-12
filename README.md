# simpledu

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

![test](https://github.com/dmfrodrigues/feup-sope-proj1/workflows/test/badge.svg)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/c5e457edb77d4b4081daa29e643ac842)](https://www.codacy.com/gh/dmfrodrigues/feup-sope-proj1/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=dmfrodrigues/feup-sope-proj1&amp;utm_campaign=Badge_Grade)

- **Project name:** simpledu
- **Short description:** Clone of Unix command `du` ([estimate file space usage](https://linux.die.net/man/1/du)), supporting most relevant options
- **Environment:** Linux
- **Tools:** C, Linux API
- **Institution:** [FEUP](https://sigarra.up.pt/feup/en/web_page.Inicial)
- **Course:** [SOPE](https://sigarra.up.pt/feup/en/UCURR_GERAL.FICHA_UC_VIEW?pv_ocorrencia_id=436440) (Operating Systems)
- **Project grade:** 100%
- **Group members:**
    - [Diogo Miguel Ferreira Rodrigues](https://github.com/dmfrodrigues) ([diogo.rodrigues@fe.up.pt](mailto:diogo.rodrigues@fe.up.pt)/[up201806429@fe.up.pt](mailto:up201806429@fe.up.pt))
    - [João António Cardoso Vieira e Basto de Sousa](https://github.com/JoaoASousa) ([up201806613@fe.up.pt](up201806613@fe.up.pt))
    - [Rafael Soares Ribeiro](https://github.com/up201806330) ([up201806330@fe.up.pt](mailto:up201806330@fe.up.pt))

Estimates file space usage.

For each subdirectory, creates a new process and communicates via pipes.

## Install

To compile the program, run `make`.

### Make commands

```sh
make          # Compile program
make test     # Run unit tests and functionality tests
make clean    # Cleanup files
make zip      # Create zip file to be submitted
make testzip  # Run tests for zip file to be submitted
```

## Usage

```
./simpledu [OPTION]... [FILE]...

Summarizes file space usage for FILE
```

### Options

```
Supports the following options, with behaviour similar to du(1):

-a, --all               write counts of all files, not just directories
--apparent-size         print apparent sizes rather than disk usage
-B, --block-size=SIZE   scale sizes by SIZE before printing
-l, --count-links       count izes many times if hard linked (activated by default, cannot be deactivated)
-L, --dereference       dereference all symbolic links
-S, --separate-dirs     for directories do not include size of subdirectories
-d, --max-depth=N       print the total for a directory only if it is in N or fewer levels below the command line argument
```

### Examples

- `./simpledu -l test/test-folder`
- `./simpledu -l test/test-folder -b`
- `./simpledu -l test/test-folder -b -S`
- `./simpledu -l test/test-folder -b -S --max-depth=2`
- `./simpledu -l test/test-folder -B 1`
- `./simpledu -l test/test-folder -a`
- `./simpledu -l test/test-folder -a -L`

## License

© 2020 Diogo Rodrigues, João António Sousa, Rafael Ribeiro

All files are licensed under [GNU General Public License v3](LICENSE), to the exception of:
- `.pdf` file, which were authored by the lecturers
- submodules, which are subject to specific licenses

The files not authored by us are presented as a fundamental complement to this project, and are made available under *fair use for education*.

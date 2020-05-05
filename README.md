# simpledu

<p align="left">
  <img src="https://github.com/dmfrodrigues/feup-sope-proj1/workflows/test/badge.svg">
</p>

- **Project name:** simpledu
- **Short description:** Clone of Unix command `du` ([estimate file space usage](https://linux.die.net/man/1/du)), supporting most relevant options
- **Institution:** [FEUP](https://sigarra.up.pt/feup/en/web_page.Inicial)
- **Course:** [SOPE](https://sigarra.up.pt/feup/en/UCURR_GERAL.FICHA_UC_VIEW?pv_ocorrencia_id=436440) (Operating Systems)
- **Project grade:** 100%
- **Group members:**
    - [Diogo Miguel Ferreira Rodrigues](https://github.com/dmfrodrigues) ([diogo.rodrigues@fe.up.pt](mailto:diogo.rodrigues@fe.up.pt)/[up201806429@fe.up.pt](mailto:up201806429@fe.up.pt))
    - [João António Cardoso Vieira e Basto de Sousa](https://github.com/JoaoASousa) ([up201806613@fe.up.pt](up201806613@fe.up.pt))
    - [Rafael Soares Ribeiro](https://github.com/up201806330) ([up201806330@fe.up.pt](mailto:up201806330@fe.up.pt))

Estimates file space usage.

For each subdirectory, creates a new process and communicates via pipes.

## Make commands
### Compile

```sh
make
```

### Test

```sh
make test
```

### Cleanup

```sh
make clean
```

### Create ZIP file

```sh
make zip
```

### Test ZIP file contents

```sh
make testzip
```

## User Commands

```sh
./simpledu [OPTION]... [FILE]...
```

Supports the following options, with behaviour similar to `du`:
- `-a, --all`
- `--apparent-size`
- `-B, --block-size=SIZE`
- `-l, --count-links` (activated by default, cannot be deactivated)
- `-L, --dereference`
- `-S, --separate-dirs`
- `-d, --max-depth=N`

May receive as many files as wanted in `[FILE]...`.

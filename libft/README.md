<h1 align="center">LIBFT</h1>

<p align="center">
    <img alt="score" src="https://img.shields.io/static/v1?label=score&message=125/100&color=brightgreen&logo=42&logoColor=green">
    <img alt="date" src="https://img.shields.io/static/v1?label=date&message=May%202nd,%202023&color=ff6984&logo=Cachet&logoColor=green">
    <img alt="size" src="https://img.shields.io/github/languages/code-size/darrenkuro/42_libft?label=size">
    <img alt="loc" src="https://img.shields.io/tokei/lines/github/darrenkuro/42_libft?label=lines">
    <img alt="file" src="https://img.shields.io/github/directory-file-count/darrenkuro/42_libft/submitted?label=files">
    <img alt="commit" src="https://img.shields.io/github/last-commit/darrenkuro/42_libft">
</p>

This repo contains files for the libft project for 42 cursus. 

`Submitted` directory contains the version of the files turned in and passed all the tests as for May 2nd, 2023. There are, however, some known issues for edge cases (malloc failing in the middle of `ft_split`) and optimization failures (i.e. allocating more memory than it needs to.)

`Updated` directory contains the changed files that are optimized after submission, therefore although theoritically better versions, it hasn't been thoroughly tested. It is intended to use for future projects and includes the bonus part natively.

`Restrcutured` directory contains the version that includes ft_printf to use for future projects.

### TODO
- [ ] Fix `ft_split` to return NULL when malloc fails in the middle of the string (one of the substr returns null.)
- [ ] Optimize `ft_substr` to malloc the exact size for when length given is larger than the source string.
- [x] Protect `ft_calloc` against multiplication overflow. Also, Malloc(0) will return a pointer that you can free.

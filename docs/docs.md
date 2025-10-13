# Pix128 documentation

Pix128 is web game project manager.<br>
It was originally made for managing *pixengine*.

## Pix128 project is made of:

- `pix128.toml` – Configuration and info about the project for pix128
- `index.html` The file where the game gets loaded in, pix128 automatically imports engine and libraries there. 
- `engine/` – Includes engine files
	- `engine/engine.toml` – Info about the engine for pix128
- `libraries/` – Includes all libraries
	- `libraries/<library>` – Includes library files
		- `libraries/<library>/library.toml` – Info about the library for pix128
- \+ other files that vary between the projects

## Pix128 network is made of:

- **[git repo]** – includes all engines, libraries, and templates
- **[local folder]** (usually .config/pix128) – includes **downloaded** or **unofficial local** engines, libraries, and templates
- **[project folder]** – made from template, includes engine and libraries the project uses


## Pix128 command cheatsheet

```bash
pix128 # shows command help and version
pix128 --help # shows command help 
pix128 --version # shows command version

pix128 engine # lists engines in [local folder]
pix128 engine download [id] # downloads engine from [git repo] to [local folder]
pix128 engine delete [id] # deletes engine from [local folder]
pix128 engine update [id] # if there is new version of the engine out, download it from [git repo] to [local folder]

pix128 library # lists libraries in [local folder]
pix128 library download [id] # downloads library from [git repo] to [local folder]
pix128 library delete [id] # deletes library from [local folder]
pix128 library update [id] # if there is new version of the library out, download it from [git repo] to [local folder]

pix128 template # lists templates in [local folder]
pix128 template download [id] # downloads template from [git repo] to [local folder]
pix128 template delete [id] # deletes template from [local folder]

pix128 new [template] [id] # makes new pix128 project from template, into the folder you are
pix128 new [template] # ("pix128 new" but asks for project id manually)
pix128 new # ("pix128 new" but asks for template and id manually)

pix128 project # shows info about project
pix128 project reload # adds javascript and css imports of libraries and engine into index.html file

pix128 project save engine # saves engine in [project folder] to [local folder], so you can use it in other projects
pix128 project save library [id] # saves library in [project folder] to [local folder], so you can use it in other projects
pix128 project save template # saves project in [project folder] as a template to [local folder], so you can make new projects using that template

pix128 project engine # shows engine used in [project folder]
pix128 project engine install [id] # installs engine from [local folder] to [project folder]
pix128 project engine reinstall # reinstalls current engine in use from [local folder] to [project folder]

pix128 project library # lists libraries used in [project folder]
pix128 project library install [id] # adds library from [local folder] to [project folder]
pix128 project library uninstall [id] # removes library from [local folder]
```
# Pix128 documentation

Pix128 is web game project manager.

It was originally made for managing *pixengine*.

# Packages

**Libraries** and **templates** are **pix128 packages**.

Every package must have `.pix128files` file with list of all files in the package, excluding `.pix128files` itself. This system allows you to download packages without a rate limit. \
<small>
(It works by http-requesting all files listed in `.pix128files` from `raw.githubusercontent.com`.
Other way to impliment this without `.pix128files` would be to use GitHub API to get list of files, but GitHub API has rate limit.)
</small>

```
index.js
css.css
README.md
```

## Libraries

Libraries are everything you can install to your project, like engines, engine extensions, assets, debug tools, etc.

Libraries can also be used to mod finished games.

Every library must have `library.toml` file.

```toml
# /libraries/example/library.toml

description = "My amazing library" # Description of this library
version = 0 # Every time you change this library, increase this number

[import]
priority = 0 # In which order files will be imported.
             # 0 = Engine
             # 1 = Something depending on engine, like engine extension
             # 2 = Something depending on something that is depending on engine
             # 3 = Something depending on something depending on something that is depending on engine
             # 5 = Mods to games
             # 10 = Debug tools
             # 20 = Assets
javascript = ["index.js"] # All of these files will be included in index.html file
css = ["style.css"] # All of these files will be included in index.html file
```

## Templates

Templates are templates of game projects.

Templates can also be used to back up games.

Every template must have `template.toml` file.

```toml
# /templates/example/template.toml

description = "My amazing template" # Description of this template
version = 0 # Every time you change this template, increase this number
```

# Pix128 network

Pix128 network is made of

- **[git repo]** – includes all offical packages (libraries, and templates).
- **[local folder]** (usually .config/pix128) – includes **downloaded** or **unofficial local** packages (libraries, and templates).
- **[project folder]** – made from template, includes libraries the project uses.

# Project

A pix128 project must have:

- `index.html` The file where the game gets loaded in. Pix128 automatically includes libraries here.
	
	CSS will be included after `<!--pix128-import-css-->` and JavaScript after `<!--pix128-import-js-->`

	```html
	<!DOCTYPE html>
	<html>
		<head>
			<title>My amazing game</title>
			<meta charset="UTF-8">
			<link rel="stylesheet" href="game.css">
			<!--pix128-import-css--><link rel="stylesheet" href="libraries/example/css.css"><link rel="stylesheet" href="libraries/example/css2.css">
		</head>
		<body>
			amazing, right!
			<script src="game.js"></script>
			<!--pix128-impor-js--><script src="libraries/example/engine.js"></script><script src="libraries/example/debug.js"></script>
		</body>
	</html>
	```
- `libraries/` – Includes all libraries

# Overview of commands

```bash
pix128 --help # shows this screen
pix128 --version # shows pix128 version

pix128 library list # lists libraries in [local folder]
pix128 library download [id] # downloads library from [git repo] to [local folder]
pix128 library delete [id] # deletes library from [local folder]
pix128 library update [id] # if there is new version of the library out, download it from [git repo] to [local folder]

pix128 template list # lists templates in [local folder]
pix128 template download [id] # downloads template from [git repo] to [local folder]
pix128 template delete [id] # deletes template from [local folder]
pix128 template update [id] # if there is new version of the library out, download it from [git repo] to [local folder]

pix128 new [template]  # makes new pix128 project from template

pix128 project reload # includes libraries to index.html

pix128 project save library [id] # saves library in [project folder] to [local folder], so you can use it in other projects
pix128 project save template # saves project in [project folder] as a template to [local folder], so you can make new projects using that template

pix128 project library # lists libraries used in [project folder]
pix128 project library install [id] # adds library from [local folder] to [project folder]
pix128 project library uninstall [id] # removes library from [local folder]
```

# Detailed descriptions of commands

## `--help`

Aliases: `-h`

Running `pix128 --help` shows list of all available commands with their descriptions.

```
Example is not ready yet.
```

You can also run this command with something before --help, like `pix128 project --help`. This shows only commands starting with `pix128 project`.

```
Example is not ready yet.
```

## `pix128 --version`

Aliases: `-v`

`pix128 --version` shows you pix128 command version.

```
Pix128 version x.x.x
```

## `pix128`

`pix128` shows you pix128 version, and tells you to use `pix128 --help` for help.

```
Pix128 version x.x.x
Run "pix128 --help" for help.
```

## `pix128 library/template`

**Reminder: Packages are libraries and templates**
	
Aliases: `pix128 library/template list`

Lists all packages in [local folder].

## `pix128 library/template download [id]`

**Reminder: Packages are libraries and templates**

Downloads package from [git repo] to [local folder].

	Does `[local folder]/package_type/id/` exist?
	
	No: Download latest version of package from [git repo].
	Yes: Use `pix128 library/template update [id]` instead.

## `pix128 library/template update`

**Reminder: Packages are libraries and templates**

Updates packages.

	Does `[local folder]/package_type/id/` exist?
		No: Asks for user permission and then downloads the package.
		Yes: Is package up-to-date?
			Yes: Asks for permission to redownload the package (if any changes made to that package, these changes will be lost).
			No: Downloads latest version from [git repo] (if any changes made to that package, these changes will be lost).

## `pix128 library/template delete`

**Reminder: Packages are libraries and templates**

	Asks for permission to delete that package.

## `New [template_id]`

Makes new pix128 project into the folder you are from template.

If template_id is empty, then it asks for it manually.

## `project reload`

Re-adds all imports to index.html.

## `project save library [id]`

If there is already library with the same name in [local folder], ask for permission to replace it.

Copy project to `[local folder]/libraries/id/`

## `project save template`

Checks if project have template.toml file.

If there is already template with the same name in [local folder], ask for permission to replace it.

Copy project to `[local folder]/templates/id/`

## `project library`

Lists all libraries installed to project.

## `project library install [id]`

Does `[project folder]/libraries/id/` already exist?

NO: Install the library from [local folder] to [project folder]

YES: Ask for permission to reinstall. (Any changes made to that library will be lost)

## `project library uninstall [id]`

Ask for permission to uninstall. (Any changes made to that library will be lost)
python -m venv -name- (creates dir)

### Command to activate virtual environment (sets VIRTUAL_ENV environment variable)
---------------------------------------
* bash/zsh
$ source <venv>/bin/activate

-----
deactivate

### Install package

pip install random-dog

### after downloading some code and wanting to run it in a venv
  python -m venv .venv
  . .venv/bin/activate (or 'source .venv/bin/activate')
  pip install -r requirements.txt

### VSCode
Status bar (bottom left corner) change to local python interpreter

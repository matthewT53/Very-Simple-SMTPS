# Create virtual environment and install all required python packages i.e meson
if [ ! -d ".project-venv" ]
then
    echo "[+] Installing pip packages..."
    python3 -m venv .project-venv
    . .project-venv/bin/activate
    pip install -r requirements.txt

    echo "[+] Installing pre-commit hook..."
    pre-commit install
    if [ ! $? -eq 0 ]
    then
        exit 127
    fi
else
    . .project-venv/bin/activate
fi

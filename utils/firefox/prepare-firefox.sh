sudo apt remove firefox
sudo add-apt-repository ppa:mozillateam/ppa
sudo sh utils/firefox/alter-firefox-package-priority.sh
sudo apt install firefox -y
pip install orjson selenium-firefox webdriver-manager

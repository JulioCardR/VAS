# VAS
A simple Account Manager for Valorant, started as a joke with my friends and then I ended up making it real, so here it is.

# Requirements
To use:
* Having Valorant installed.
* Going through the setup and adding your accounts.

To make your own build:
* VAS uses CryptoPP for hashing and encryption.
* It also uses windows libraries.

## FAQ
# What it does?
VAS purpose is to make it easier to change between accounts on the game Valorant. All you have to do is type your master password and choose the account you want to use.

# How it works?
VAS creates a configuration file that stores the account informations, the master password and the location of the Riot Client. Whenever you choose what account you want to use VAS will open the game for you and simulate keystrokes to type your username and password for you.

# Is it safe?
All data is stored locally. Account information and the master password uses AES 256 encryption. That being said, the encryption wasn't implemented flawlessly, if someone has access to you machine and really wants your account information they would problably be able to get it.

# I don't remeber my master password, what should I do?
Just delete the VAS.conf file, go through the setup and add your accounts again. VAS is not there to remeber your account information, it's just a tool to make it easier for you to login.

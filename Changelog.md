# Changelog
All notable changes to this project will be documented in this file.

Format for version is major.minor.patch, where 0 is reserved in minor for alpha release, 1 for beta release and then more stable the higher it goes.

## [2.0.0] - Sunday, November 7th 2021 

- Radio logger system written
- Config file capability added
- Simulation of radios ability added (set in config file)
- Update of build mechanics to be more fluid

## [1.0.0] - Friday, October 2nd 2020 

- Initial release of service - installed at Ruby
- Firmware version polling enabled
- Boot into an available firmware version enabled
- Upload new versions of firmware enabled
- Basic timeouts enabled for link down in the middle of one of the above operations
- Program enabled as service - starts on raspberry pi reboot and auto restarts if there is a crash
- Changed RPATH so spdlog works
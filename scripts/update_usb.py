Import("env")

board_config = env.BoardConfig()

# should be array of VID:PID pairs
board_config.update("build.usb_product", "DINoctopus")
board_config.update("name", "DINoctopus")

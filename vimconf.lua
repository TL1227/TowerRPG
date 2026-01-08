--keymaps
vim.keymap.set("n", "<c-f>", ":vimgrep // src/*<left><left><left><left><left><left><left>")
vim.keymap.set("n", "<leader>ff", ":find ")

--options
vim.opt.path = {"src/", "shaders/"}
vim.opt.suffixesadd = {".cpp", ".h", ".shader"}

--misc
require("config.lazy")
vim.cmd("color vscode")

print("Crimson Tower Vimconf loaded")


local testFailMessages = {
	"HOLY COW BADMAN TEST FAIL",
	"GREAT SCOTT THIS TEST HAS GONE SOUTH",
	"GADZOOKS TEST FAIL"
}

-- http://stackoverflow.com/questions/5303174/get-list-of-directory-in-a-lua
-- Lua implementation of PHP scandir function
function scandir(directory)
    local i, t, popen = 0, {}, io.popen
    for filename in popen('ls -a "'..directory..'"'):lines() do
    	if filename ~= '.' and filename ~= '..' then
        	i = i + 1
        	t[i] = filename
        end
    end
    return t
end

local testCount = 0
local testPassCount = 0

for _,v in pairs(scandir("test")) do
	if v:find(".test") ~= nil then
		local filename = "test/" .. v
		io.input(filename)
		local testFile = "test/" .. io.read("*line")
		local expectedOut = io.read("*all")
		local programOut = io.popen("./jlang -i " .. testFile):read("*all")
		local interpOut = programOut:gsub(".*INTERP RESULT:", ""):gsub("%s", "")
		if expectedOut == interpOut then
			testPassCount = testPassCount + 1
			print(testFile ..  "\t\t.... passed")
		else
			print(testFile, ".... FAILED!")
			print("EXPECTED: " .. expectedOut)
			print("ACTUAL: " .. interpOut)
		end
		testCount = testCount + 1
	end
end

print("Tests passed " .. testPassCount .. "/" .. testCount)
if testPassCount < testCount then
	print("GREAT SCOTT THIS TEST HAS GONE SOUTH OLD CHAP")
end
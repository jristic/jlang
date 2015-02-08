
math.randomseed( os.time() )

local testFailMessages = {
	"HOLY COW BADMAN TEST FAIL",
	"GREAT SCOTT THIS TEST HAS GONE SOUTH",
	"GADZOOKS TEST FAIL",
	"TESTY MCTESTERSON HAS FAILED IN HIS QUEST",
	"WHAT A HORRIBLE NIGHT TO HAVE A TEST FAIL",
	"NaNNanNaNNaNNaN WATMAN (oh, right, that's why we're here... a test failed)",
	"BABABOOIE BABABOOIE BABABOOIE BABABOOIE",
	"SHOES SHOES SHOES FAIL"
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
		-- The extra newline is to match the program
		local expectedOut = io.read("*all") .. "\n" 
		local programOut = io.popen("./jlang -i " .. testFile):read("*all")
		local interpOut = programOut:gsub(".*INTERP RESULT:\n", "")
		if expectedOut == interpOut then
			testPassCount = testPassCount + 1
			print(testFile ..  "\t\t.... passed")
		else
			print(testFile, ".... FAILED!")
			print("EXPECTED: \n" .. expectedOut)
			print("ACTUAL: \n" .. interpOut)
		end
		testCount = testCount + 1
	end
end

print("Tests passed " .. testPassCount .. "/" .. testCount)
if testPassCount < testCount then
	print(testFailMessages[math.random(#testFailMessages)])
end
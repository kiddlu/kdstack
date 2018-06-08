[void][reflection.assembly]::LoadFile(".\math.dll")  
[Math.methods]::CompareI(10,2)  
$a=New-Object Math.Methods  
$a.CompareII(2,3)

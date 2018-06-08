;;WARNING NOTE 1: does't count grouped open folders
;;WARNING NOTE 2: opened Windows Media Player counts as 4 instances
;;WARNING NOTE 3: count fails in the remote hypothesis that, for example, a folder named ".pdf - Adobe Reader" is open 
;; based on http://www.autohotkey.com/board/topic/35867-winget-count-is-telling-me-i-have-6-windows-open-but/

#SingleInstance, force 

Gui, add, text, , Windows count (including not grouped ones) = 
Gui, add, text, x+5 vcount, XX 
Gui, add, edit, xm r20 vWList w400 -wrap +0x100000 +readonly 
Gui, Show
GoSub, checklist

SetTimer, checklist, 3000 
return 

checklist: 
  WinGet, count, count
  WinGet,current,List 
  nocount = 0
  last_app_name = %A_Space%- %A_Tab% ; something that surely can't be used for files/folders file... Tip: anyway "Start" should always be the first element of the list
  myArray := Object()
  log = grouped applications instances:
  loop, %current% { 
    WinGetTitle, tTitle, % "ahk_id " current%A_Index% 
    if tTitle is space
    {
      nocount += 1 
    }
    else if (tTitle == A_ScriptName || tTitle == "Start" || tTitle == "Program Manager" || tTitle == "Default IME" || tTitle == "HTA Test" || tTitle = "C:\Windows\system32\cmd.exe") ; note == for everything except = path as case may vary on different PCs
    {
      nocount += 1
    }
    else
    {
     temp_FoundP := InStr(tTitle, last_app_name, 0) 
     if ((temp_FoundP != 0 ) && (last_app_name == substr(tTitle, temp_FoundP) ))
     {
       myArray[last_app_name] += 1
     }
     else
     {
      temp_FoundP := InStr(tTitle, " - ", 0) 
      if (temp_FoundP != 0) {
        last_app_name := substr(tTitle, temp_FoundP)
      }
      else {
        last_app_name = %A_Space%- %A_Tab%
      }
      if myArray[last_app_name] {
        myArray[last_app_name] += 1
      }
      else { myArray[last_app_name] := 1
      }
     }
    } 
   } 
  remaindercount = 0
  For key, value in myArray
  {
;MsgBox %key% = %value% ; DELETE THIS LINE
    if (key == "-" || value == 1) {
      remaindercount += value
    }
    else {    
      log .= "`n" key " = " value 
    }
  }
  if remaindercount > 0
      log .= "`n(remaining = " remaindercount ")"
;  Sort, log, C   ; uncomment this to sort by name 
  count -= nocount
  GuiControl, , count, %count%
  GuiControl, , WList, %log% 
  return 

GuiClose: 
ExitApp
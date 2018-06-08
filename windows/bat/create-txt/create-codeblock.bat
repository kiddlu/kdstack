@echo off  
rem use ^ symble for echo "Escape"
set ECHO_CMD=echo;
 
rem standalone codeblock
(
%ECHO_CMD% filters:
%ECHO_CMD% #print all
%ECHO_CMD%     - ^^*
%ECHO_CMD% #print Error only
%ECHO_CMD% #    - ^^E/
%ECHO_CMD% #print art
%ECHO_CMD% #    - ^^./art
%ECHO_CMD% #print I/DEBUG
%ECHO_CMD% #    - ^^I/DEBUG
%ECHO_CMD% 
%ECHO_CMD% highlights:
%ECHO_CMD%   "^I/.*": g
%ECHO_CMD%   "^E/.*": r
%ECHO_CMD%   "^W/.*": y
%ECHO_CMD%   "^D/.*": b
%ECHO_CMD%   "^V/.*": c
%ECHO_CMD%   "^*kernel": r
) > %~dp0/config.yaml

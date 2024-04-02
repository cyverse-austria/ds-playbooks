# All of the customizations specific to CyVerse Austria are in this rule base.
# include this file from within ipc-custom.re

@include 'at-env'


at_acPreProcForDeleteUser {
  if ($otherUserZone == '') {
    *zone = ipc_ZONE;
    *home = $otherUserName;
  } else {
    *zone = $otherUserZone;
    *home = $otherUserName ++ '#' ++ $otherUserZone;
  }

  *id = '';

  foreach (*row in SELECT USER_ID WHERE USER_NAME == $otherUserName AND USER_ZONE == *zone) {
    *id = *row.USER_ID;
  }

  if (*id == '') {
    cut;
    fail(-827000);
  }

  *homePath = '/' ++ ipc_ZONE ++ '/home/' ++ *home;
  *retiredPath = '/' ++ ipc_ZONE ++ '/retired/' ++ *home ++ '-' ++ *id;
  *argStr = execCmdArg(*homePath) ++ ' ' ++ execCmdArg(*retiredPath);
  *status = errorcode(msiExecCmd('at-mv-coll', *argStr, ipc_RE_HOST, 'null', 'null', *out));

  if (*status < 0) {
    msiGetStderrInExecCmdOut(*out, *err);
    cut;
    failmsg(*status, 'Unable to rename *homePath to *retiredPath - *err');
  }

  *curUserArg = execCmdArg($otherUserName);
  *curZoneArg = execCmdArg(*zone);
  *newUserArg = execCmdArg(at_ADMIN_USER);
  *newZoneArg = execCmdArg(ipc_ZONE);
  *argStr = '*curUserArg *curZoneArg *newUserArg *newZoneArg';
  *status = errorcode(msiExecCmd('at-switch-own', *argStr, ipc_RE_HOST, 'null', 'null', *out));

  if (*status < 0) {
    msiGetStderrInExecCmdOut(*out, *err);

    *msg = 'Unable to switch owner of ' ++ *retiredPath ++ ' to ' ++ at_ADMIN_USER ++ '#'
         ++ ipc_ZONE ++ ' - "' ++ *err ++ '"';

    writeLine('serverLog', *msg);
  }
}

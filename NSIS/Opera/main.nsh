# Opera

    # '/silent /launchopera 0 /allusers' are always selected. Not doing so
    # would not make sense in an environment for automated installations.

    ExecWait '"${Opera}" /silent /launchopera 0 /allusers'

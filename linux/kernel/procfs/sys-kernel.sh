#!/bin/bash



cat << "EOF"
├── acct
├── acpi_video_flags
├── auto_msgmni
├── bootloader_type
├── bootloader_version
├── cad_pid
├── cap_last_cap
├── compat-log
├── core_pattern
├── core_pipe_limit
├── core_uses_pid
├── ctrl-alt-del
├── dmesg_restrict
├── domainname
├── ftrace_dump_on_oops
├── ftrace_enabled
├── hostname
├── hotplug
├── hung_task_check_count
├── hung_task_panic
├── hung_task_timeout_secs
├── hung_task_warnings
├── io_delay_type
├── kexec_load_disabled
├── keys
│   ├── gc_delay
│   ├── maxbytes
│   ├── maxkeys
│   ├── persistent_keyring_expiry
│   ├── root_maxbytes
│   └── root_maxkeys
├── kptr_restrict
├── kstack_depth_to_print
├── max_lock_depth
├── modprobe
├── modules_disabled
├── msgmax
├── msgmnb
├── msgmni
├── msg_next_id
├── ngroups_max
├── nmi_watchdog
├── ns_last_pid
├── numa_balancing
├── numa_balancing_scan_delay_ms
├── numa_balancing_scan_period_max_ms
├── numa_balancing_scan_period_min_ms
├── numa_balancing_scan_size_mb
├── osrelease
├── ostype
├── overflowgid
├── overflowuid
├── panic
├── panic_on_io_nmi
├── panic_on_oops
├── panic_on_unrecovered_nmi
├── panic_on_warn
├── perf_cpu_time_max_percent
├── perf_event_max_sample_rate
├── perf_event_mlock_kb
├── perf_event_paranoid
├── pid_max
├── poweroff_cmd
├── print-fatal-signals
├── printk
├── printk_delay
├── printk_ratelimit
├── printk_ratelimit_burst
├── pty
│   ├── max
│   ├── nr
│   └── reserve
├── random
│   ├── boot_id
│   ├── entropy_avail
│   ├── poolsize
│   ├── read_wakeup_threshold
│   ├── urandom_min_reseed_secs
│   ├── uuid
│   └── write_wakeup_threshold
├── randomize_va_space
├── real-root-dev
├── sched_autogroup_enabled
├── sched_cfs_bandwidth_slice_us
├── sched_child_runs_first
├── sched_domain
│   ├── cpu0
│   │   ├── domain0
│   │   │   ├── busy_factor
│   │   │   ├── busy_idx
│   │   │   ├── cache_nice_tries
│   │   │   ├── flags
│   │   │   ├── forkexec_idx
│   │   │   ├── idle_idx
│   │   │   ├── imbalance_pct
│   │   │   ├── max_interval
│   │   │   ├── max_newidle_lb_cost
│   │   │   ├── min_interval
│   │   │   ├── name
│   │   │   ├── newidle_idx
│   │   │   └── wake_idx
│   │   └── domain1
│   │       ├── busy_factor
│   │       ├── busy_idx
│   │       ├── cache_nice_tries
│   │       ├── flags
│   │       ├── forkexec_idx
│   │       ├── idle_idx
│   │       ├── imbalance_pct
│   │       ├── max_interval
│   │       ├── max_newidle_lb_cost
│   │       ├── min_interval
│   │       ├── name
│   │       ├── newidle_idx
│   │       └── wake_idx
│   ├── cpu1
│   │   ├── domain0
│   │   │   ├── busy_factor
│   │   │   ├── busy_idx
│   │   │   ├── cache_nice_tries
│   │   │   ├── flags
│   │   │   ├── forkexec_idx
│   │   │   ├── idle_idx
│   │   │   ├── imbalance_pct
│   │   │   ├── max_interval
│   │   │   ├── max_newidle_lb_cost
│   │   │   ├── min_interval
│   │   │   ├── name
│   │   │   ├── newidle_idx
│   │   │   └── wake_idx
│   │   └── domain1
│   │       ├── busy_factor
│   │       ├── busy_idx
│   │       ├── cache_nice_tries
│   │       ├── flags
│   │       ├── forkexec_idx
│   │       ├── idle_idx
│   │       ├── imbalance_pct
│   │       ├── max_interval
│   │       ├── max_newidle_lb_cost
│   │       ├── min_interval
│   │       ├── name
│   │       ├── newidle_idx
│   │       └── wake_idx
│   ├── cpu2
│   │   ├── domain0
│   │   │   ├── busy_factor
│   │   │   ├── busy_idx
│   │   │   ├── cache_nice_tries
│   │   │   ├── flags
│   │   │   ├── forkexec_idx
│   │   │   ├── idle_idx
│   │   │   ├── imbalance_pct
│   │   │   ├── max_interval
│   │   │   ├── max_newidle_lb_cost
│   │   │   ├── min_interval
│   │   │   ├── name
│   │   │   ├── newidle_idx
│   │   │   └── wake_idx
│   │   └── domain1
│   │       ├── busy_factor
│   │       ├── busy_idx
│   │       ├── cache_nice_tries
│   │       ├── flags
│   │       ├── forkexec_idx
│   │       ├── idle_idx
│   │       ├── imbalance_pct
│   │       ├── max_interval
│   │       ├── max_newidle_lb_cost
│   │       ├── min_interval
│   │       ├── name
│   │       ├── newidle_idx
│   │       └── wake_idx
│   ├── cpu3
│   │   ├── domain0
│   │   │   ├── busy_factor
│   │   │   ├── busy_idx
│   │   │   ├── cache_nice_tries
│   │   │   ├── flags
│   │   │   ├── forkexec_idx
│   │   │   ├── idle_idx
│   │   │   ├── imbalance_pct
│   │   │   ├── max_interval
│   │   │   ├── max_newidle_lb_cost
│   │   │   ├── min_interval
│   │   │   ├── name
│   │   │   ├── newidle_idx
│   │   │   └── wake_idx
│   │   └── domain1
│   │       ├── busy_factor
│   │       ├── busy_idx
│   │       ├── cache_nice_tries
│   │       ├── flags
│   │       ├── forkexec_idx
│   │       ├── idle_idx
│   │       ├── imbalance_pct
│   │       ├── max_interval
│   │       ├── max_newidle_lb_cost
│   │       ├── min_interval
│   │       ├── name
│   │       ├── newidle_idx
│   │       └── wake_idx
│   ├── cpu4
│   │   ├── domain0
│   │   │   ├── busy_factor
│   │   │   ├── busy_idx
│   │   │   ├── cache_nice_tries
│   │   │   ├── flags
│   │   │   ├── forkexec_idx
│   │   │   ├── idle_idx
│   │   │   ├── imbalance_pct
│   │   │   ├── max_interval
│   │   │   ├── max_newidle_lb_cost
│   │   │   ├── min_interval
│   │   │   ├── name
│   │   │   ├── newidle_idx
│   │   │   └── wake_idx
│   │   └── domain1
│   │       ├── busy_factor
│   │       ├── busy_idx
│   │       ├── cache_nice_tries
│   │       ├── flags
│   │       ├── forkexec_idx
│   │       ├── idle_idx
│   │       ├── imbalance_pct
│   │       ├── max_interval
│   │       ├── max_newidle_lb_cost
│   │       ├── min_interval
│   │       ├── name
│   │       ├── newidle_idx
│   │       └── wake_idx
│   ├── cpu5
│   │   ├── domain0
│   │   │   ├── busy_factor
│   │   │   ├── busy_idx
│   │   │   ├── cache_nice_tries
│   │   │   ├── flags
│   │   │   ├── forkexec_idx
│   │   │   ├── idle_idx
│   │   │   ├── imbalance_pct
│   │   │   ├── max_interval
│   │   │   ├── max_newidle_lb_cost
│   │   │   ├── min_interval
│   │   │   ├── name
│   │   │   ├── newidle_idx
│   │   │   └── wake_idx
│   │   └── domain1
│   │       ├── busy_factor
│   │       ├── busy_idx
│   │       ├── cache_nice_tries
│   │       ├── flags
│   │       ├── forkexec_idx
│   │       ├── idle_idx
│   │       ├── imbalance_pct
│   │       ├── max_interval
│   │       ├── max_newidle_lb_cost
│   │       ├── min_interval
│   │       ├── name
│   │       ├── newidle_idx
│   │       └── wake_idx
│   ├── cpu6
│   │   ├── domain0
│   │   │   ├── busy_factor
│   │   │   ├── busy_idx
│   │   │   ├── cache_nice_tries
│   │   │   ├── flags
│   │   │   ├── forkexec_idx
│   │   │   ├── idle_idx
│   │   │   ├── imbalance_pct
│   │   │   ├── max_interval
│   │   │   ├── max_newidle_lb_cost
│   │   │   ├── min_interval
│   │   │   ├── name
│   │   │   ├── newidle_idx
│   │   │   └── wake_idx
│   │   └── domain1
│   │       ├── busy_factor
│   │       ├── busy_idx
│   │       ├── cache_nice_tries
│   │       ├── flags
│   │       ├── forkexec_idx
│   │       ├── idle_idx
│   │       ├── imbalance_pct
│   │       ├── max_interval
│   │       ├── max_newidle_lb_cost
│   │       ├── min_interval
│   │       ├── name
│   │       ├── newidle_idx
│   │       └── wake_idx
│   └── cpu7
│       ├── domain0
│       │   ├── busy_factor
│       │   ├── busy_idx
│       │   ├── cache_nice_tries
│       │   ├── flags
│       │   ├── forkexec_idx
│       │   ├── idle_idx
│       │   ├── imbalance_pct
│       │   ├── max_interval
│       │   ├── max_newidle_lb_cost
│       │   ├── min_interval
│       │   ├── name
│       │   ├── newidle_idx
│       │   └── wake_idx
│       └── domain1
│           ├── busy_factor
│           ├── busy_idx
│           ├── cache_nice_tries
│           ├── flags
│           ├── forkexec_idx
│           ├── idle_idx
│           ├── imbalance_pct
│           ├── max_interval
│           ├── max_newidle_lb_cost
│           ├── min_interval
│           ├── name
│           ├── newidle_idx
│           └── wake_idx
├── sched_latency_ns
├── sched_migration_cost_ns
├── sched_min_granularity_ns
├── sched_nr_migrate
├── sched_rr_timeslice_ms
├── sched_rt_period_us
├── sched_rt_runtime_us
├── sched_shares_window_ns
├── sched_time_avg_ms
├── sched_tunable_scaling
├── sched_wakeup_granularity_ns
├── sem
├── sem_next_id
├── sg-big-buff
├── shmall
├── shmmax
├── shmmni
├── shm_next_id
├── shm_rmid_forced
├── softlockup_all_cpu_backtrace
├── softlockup_panic
├── soft_watchdog
├── stack_tracer_enabled
├── sysctl_writes_strict
├── sysrq
├── tainted
├── threads-max
├── timer_migration
├── traceoff_on_warning
├── tracepoint_printk
├── unknown_nmi_panic
├── usermodehelper
│   ├── bset
│   └── inheritable
├── version
├── watchdog
├── watchdog_cpumask
├── watchdog_thresh
└── yama
    └── ptrace_scope
EOF

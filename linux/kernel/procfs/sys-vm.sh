#!/bin/bash


cat << "EOF"
├── admin_reserve_kbytes
├── block_dump
├── compact_memory
├── compact_unevictable_allowed
├── dirty_background_bytes
├── dirty_background_ratio
├── dirty_bytes
├── dirty_expire_centisecs
├── dirty_ratio
├── dirtytime_expire_seconds
├── dirty_writeback_centisecs
├── drop_caches
├── extfrag_threshold
├── hugepages_treat_as_movable
├── hugetlb_shm_group
├── laptop_mode
├── legacy_va_layout
├── lowmem_reserve_ratio
├── max_map_count
├── memory_failure_early_kill
├── memory_failure_recovery
├── min_free_kbytes
├── min_slab_ratio
├── min_unmapped_ratio
├── mmap_min_addr
├── nr_hugepages
├── nr_hugepages_mempolicy
├── nr_overcommit_hugepages
├── nr_pdflush_threads
├── numa_zonelist_order
├── oom_dump_tasks
├── oom_kill_allocating_task
├── overcommit_kbytes
├── overcommit_memory
├── overcommit_ratio
├── page-cluster
├── panic_on_oom
├── percpu_pagelist_fraction
├── stat_interval
├── swappiness
├── user_reserve_kbytes
├── vfs_cache_pressure
└── zone_reclaim_mode
EOF

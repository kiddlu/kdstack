|                                          | O     | Go            | Go-512 |      |
| ---------------------------------------- | ----- | ------------- | ------ | ---- |
| system property                          | *     | *             |        |      |
| ro.config.low_ram                        | -     | true          | @      |      |
| ro.lmk.medium                            | -     | -             | 700    |      |
| ro.lmk.critical_upgrade                  | -     | true          | @      |      |
| ro.lmk.upgrade_pressure                  | -     | true          | @      |      |
| pm.dexopt.shared                         | speed | quicken       | @      |      |
| pm.dexopt.downgrade_after_inactive_days  | -     | 10            | @      |      |
| dalvik.vm.heapgrowthlimit                |       | 128m          | @      |      |
| dalvik.vm.heapsize                       |       | 256m          | @      |      |
| dalvik.vm.systemservercompilerfilter     |       | speed-profile | @      |      |
| dalvik.vm.madvise-random                 |       | -             | true   |      |
| dalvik.vm.jit.codecachesize              |       | 0             | @      |      |
| dalvik.vm.foreground-heap-growth-multiplier(suggest) | 1.0   | 2.0           | 1.0    |      |
|                                          |       |               |        |      |
|                                          |       |               |        |      |
|                                          |       |               |        |      |
| MALLOC_SVELTE: external/jemalloc         |       |               |        |      |
|                                          |       |               |        |      |


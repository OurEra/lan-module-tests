## AlrDetector

AlrDetector 用于检查当前应用发送出去的数据, 是否低于当前带宽值的某个门限.

> static constexpr int kDefaultAlrBandwidthUsagePercent = 65;
> static constexpr int kDefaultAlrStartBudgetLevelPercent = 80;
> static constexpr int kDefaultAlrStopBudgetLevelPercent = 50;


当带宽值更新时,通过 AlrDetector::SetEstimatedBitrate() 更新到该模块, 当应用发送数据时, 通过
AlrDetector::OnBytesSent() 来表示.

具体计算在 IntervalBudget 中完成, IntervalBudget::set_target_rate_kbps() 用与接收当前计算的带宽,
并计算一个 500ms (kWindowMs) 内应用应该发送的数据大小 max_bytes_in_budget_; 

当应用发送数据时, IntervalBudget::IncreaseBudget() 根据经过的 delta 时间, 来更新应用 delta 内需要发送出去的数据量
到 bytes_remaining_, 然后 IntervalBudget::UseBudget() 则会根据实际数据减少 bytes_remaining_, 当 bytes_remaining_ 和
max_bytes_in_budget_ 的比值超过一定门限, 则认为是应用自己的限制导致了没有发送足够的数据.



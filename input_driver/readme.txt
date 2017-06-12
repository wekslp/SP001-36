kernel panic 需注意所有function是否有傳回值
1. 透過修改cdata-ts.c來實作input device driver的實作
2. 移除cdata-ts.c相關不必要的硬體實作的程式碼
3. cdata-ts 註冊後，在應用程式open()後，即註冊一個虛擬的input device(/sys/devices/virtual/xxx)
4. input_report_abs() 是否為atomic()
5. 實作cdata_ts_ioctl, 並透過input_report_abs()來傳送資料。
6. 新增read_event.c 來讀取/dev/input/eventX的event


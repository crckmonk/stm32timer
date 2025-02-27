@startuml
[*] --> Init : Start

state Idle {
    [*] --> SetCountDown : Encoder changes, !cdCountDown.isOn
    [*] --> StartCountDown : SW_Pin low, !cdCountDown.isOn
    [*] --> Counting : SW_Pin low, cdCountDown.isOn
    [*] --> DisplayUpdate : tm1637_Display_Handle()
}

state SetCountDown {
    [*] --> Idle : countDown_Set()
}

state StartCountDown {
    [*] --> Counting : countDown_Start()
}

state Counting {
    [*] --> PauseCountDown : SW_Pin low, cdCountDown.isOn
    [*] --> Tick : HAL_TIM_PeriodElapsedCallback()
    [*] --> DisplayUpdate : tm1637_Display_Handle()
}

state PauseCountDown {
    [*] --> Idle : countDown_Pause()
    [*] --> DisplayUpdate : tm1637_Display_Handle()
}

state Tick {
    [*] --> Counting : cdCountDown.secLeftTotal > 0, countDown_Tick(1)
    [*] --> Done : cdCountDown.secLeftTotal <= 0, countDown_Tick(1)
}

state Done {
    [*] --> Blinking : cdCountDown.isDone < 50
    [*] --> Idle : cdCountDown.isDone >= 50
    [*] --> DisplayUpdate : tm1637_Display_Handle()
}

state Blinking {
    [*] --> Blinking : cdCountDown.isDone < 50, HAL_Delay()
    [*] --> Idle : cdCountDown.isDone >= 50
    [*] --> DisplayUpdate : tm1637_Display_Handle()
}

@enduml
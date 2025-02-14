#pragma once
#include <Arduino.h>

namespace sets {

enum class Code : uint8_t {
    id,
    id2,
    type,
    content,
    group,
    menu,
    buttons,
    row,

    build,
    reload,
    update,
    update_tout,
    slider_tout,
    request_tout,
    granted,
    fs,
    error,
    gzip,
    used,
    total,
    rssi,
    custom_hash,

    label,
    title,
    text,
    value,
    data,
    color,
    min,
    max,
    step,
    unit,
    color_on,
    color_off,

    input,
    number,
    pass,
    select,
    slider,
    slider2,
    toggle,
    time,
    date,
    datetime,
    button,
    button_hold,
    paragraph,
    confirm,
    led,
    alert,
    notice,
    log,
    proj_name,
    proj_link,
    divtype,
    line,
    block,
    html,
    regex,
    format,
    maxlen,
    zone,
    image,
    stream,
    tabs,
    ws_port,
    plot_run,
    plot_stack,
    plot,
    plot_time,
    period,
    tmode,
    uptime,
    joystick,
    center,
};

}
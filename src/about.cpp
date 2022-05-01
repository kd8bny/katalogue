// SPDX-License-Identifier: GPL-2.0-or-later
// PDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

#include "about.h"

KAboutData AboutType::aboutData() const
{
    return KAboutData::applicationData();
}

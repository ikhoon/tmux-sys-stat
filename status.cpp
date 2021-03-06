/*
 * status.cpp
 *
 * Cpu/Mem/LoadAvg/... status.
 *
 * Copyright (C) 2012 Jeanluc Chasseriau <jeanluc.chasseriau@crossing-tech.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "status.h"

#include <stdio.h>
#include <math.h>

#include "cpureader.h"
#include "memreader.h"
#include "loadaveragereader.h"

using namespace std;

StatusVisitor::StatusVisitor() {
  cpuStatus_ = "";
  memStatus_ = "";
  lavgStatus_ = "";
}

StatusVisitor::~StatusVisitor() {
}

void StatusVisitor::printStatus() {
  printf("%s %s %s\n", memStatus_.c_str(), cpuStatus_.c_str(), lavgStatus_.c_str());
}

void StatusVisitor::visit(CpuReader* cpu) {
  double percentUsed = 0.0;
  double nbBar = 0.0;
  double nbEmptyBar = 0.0;

  char *cur = NULL;

  int i = cpu->cpuCount();
  int j = 0;

  const char *formatPercent         = "%02.0F%%";
  const char *formatPercentSpace    = "%02.0F%% ";

  double totalUsed = 0.0;

  while (i--) {
    percentUsed = (cpu->statSystem(i) + cpu->statUser(i)) * 100.0;
    totalUsed += percentUsed;
  }
  
  int scale = 2;
  nbBar = ceil(totalUsed / (PERCENT_PER_BAR * scale));
  nbEmptyBar = NB_BAR_MAX * scale - (int)nbBar;

  cur = tmpOutput;
  j = 0;
  *cur++ = '[';
  j = nbBar;
  while (j--)   { *cur++ = '|'; }
  j = nbEmptyBar;
  while (j--)   { *cur++ = ' '; }
  *cur++ = ']';
  sprintf(cur, "%5.1F%%", totalUsed / cpu->cpuCount());
  cpuStatus_ += tmpOutput;

}

void StatusVisitor::visit(MemReader* mem) {
  double percentMemUsed = (100.0 * (mem->totalMB()-mem->freeMB())) / mem->totalMB();
  int totalMB = mem->totalMB() / (1024 * 1024);
  int freeMB = mem->freeMB() / (1024 * 1024);
  sprintf(tmpOutput, "%0.1F%% Mem, %d/%dMB", percentMemUsed, freeMB, totalMB);
  memStatus_ += tmpOutput;
}

void StatusVisitor::visit(LoadAverageReader* lavg) {
//  sprintf(tmpOutput, "%0.2F %0.2F %0.2F", lavg->loadAverage1(), lavg->loadAverage5(), lavg->loadAverage15());
//  lavgStatus_ = tmpOutput;
}



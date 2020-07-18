/*
 *      Copyright (C) 2020 Team Kodi
 *      http://kodi.tv
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this Program; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "MemoryMap.h"

using namespace LIBRETRO;

size_t CMemoryMap::GetNumOfDescriptors()
{
  return m_memoryDescriptors.size();
}

void CMemoryMap::AddDescriptor(const retro_memory_descriptor& descriptor)
{
  m_memoryDescriptors.push_back({descriptor, 0});
}

bool CMemoryMap::PreprocessDescriptors()
{
  size_t top_addr = 1;
  for (auto& desc : m_memoryDescriptors)
  {
    if (desc.descriptor.select != 0)
      top_addr |= desc.descriptor.select;
    else
      top_addr |= desc.descriptor.start + desc.descriptor.len - 1;
  }

  top_addr = AddBitsDown(top_addr);

  for (auto& desc : m_memoryDescriptors)
  {
    if (desc.descriptor.select == 0)
    {
      if (desc.descriptor.len == 0)
        return false;

      if ((desc.descriptor.len & (desc.descriptor.len - 1)) != 0)
        return false;

      desc.descriptor.select = top_addr & ~Inflate(AddBitsDown(desc.descriptor.len - 1),
                                                      desc.descriptor.disconnect);
    }

    if (desc.descriptor.len == 0)
    {
      desc.descriptor.len = AddBitsDown(Reduce(top_addr & ~desc.descriptor.select,
                                                  desc.descriptor.disconnect)) + 1;
    }

    if (desc.descriptor.start & ~desc.descriptor.select)
      return false;

    while (Reduce(top_addr & ~desc.descriptor.select, desc.descriptor.disconnect) >> 1 >
           desc.descriptor.len - 1)
    {
      desc.descriptor.disconnect |=
          HighestBit(top_addr & ~desc.descriptor.select & ~desc.descriptor.disconnect);
    }

    desc.disconnectMask = AddBitsDown(desc.descriptor.len - 1);
    desc.descriptor.disconnect &= desc.disconnectMask;

    while ((~desc.disconnectMask) >> 1 & desc.descriptor.disconnect)
    {
      desc.disconnectMask >>= 1;
      desc.descriptor.disconnect &= desc.disconnectMask;
    }
  }

  return true;
}

size_t CMemoryMap::AddBitsDown(size_t num)
{
  num |= num >> 1;
  num |= num >> 2;
  num |= num >> 4;
  num |= num >> 8;
  num |= num >> 16;

  if (sizeof(size_t) > 4)
    num |= num >> 16 >> 16;

  return num;
}

size_t CMemoryMap::Inflate(size_t addr, size_t mask)
{
  while (mask)
  {
    size_t tmp = (mask - 1) & ~mask;
    addr = ((addr & ~tmp) << 1) | (addr & tmp);
    mask = mask & (mask - 1);
  }

  return addr;
}

size_t CMemoryMap::Reduce(size_t addr, size_t mask)
{
  while (mask)
  {
    size_t tmp = (mask - 1) & ~mask;
    addr = (addr & tmp) | ((addr >> 1) & ~tmp);
    mask = (mask & (mask - 1)) >> 1;
  }

  return addr;
}

size_t CMemoryMap::HighestBit(size_t num)
{
  num = AddBitsDown(num);
  return num ^ (num >> 1);
}

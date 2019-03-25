// Copyright (C) 2019 Chris Richardson
//
// This file is part of DOLFIN (https://www.fenicsproject.org)
//
// SPDX-License-Identifier:    LGPL-3.0-or-later

#include <dolfin/common/types.h>
#include <memory>
#include <ufc.h>
#include <vector>

#pragma once

struct ufc_dofmap;

namespace dolfin
{

namespace mesh
{
class CellType;
}

namespace fem
{

/// Element degree-of-freedom map

/// This class handles the mapping of degrees of freedom on an element.

class ElementDofMap
{
public:
  /// Constructor from UFC dofmap
  ElementDofMap(const ufc_dofmap& ufc_dofmap, const mesh::CellType& cell_type);

public:
  // Copy constructor
  ElementDofMap(const ElementDofMap& dofmap) = delete;

  /// Move constructor
  ElementDofMap(ElementDofMap&& dofmap) = default;

  /// Destructor
  ~ElementDofMap() = default;

  ElementDofMap& operator=(const ElementDofMap& dofmap) = delete;

  /// Move assignment
  ElementDofMap& operator=(ElementDofMap&& dofmap) = default;

  /// Number of dofs associated with entities of dimension dim
  int num_entity_dofs(int dim) const
  {
    assert(dim < 4);
    return _num_entity_dofs[dim];
  }

  /// Tabulate dofs on a specific entity i of dimension dim
  std::vector<int> tabulate_entity_dofs(int dim, int i) const;

  /// Get number of sub-dofmaps
  unsigned int num_sub_dofmaps() const { return sub_dofmaps.size(); }

  /// Get subdofmap i
  const ElementDofMap& sub_dofmap(unsigned int i) const
  {
    assert(i < sub_dofmaps.size());
    return *sub_dofmaps[i];
  }

private:
  // Total number of dofs in this element dofmap
  int _cell_dimension;

  // The number of dofs associated with each entity type
  int _num_entity_dofs[4];

  // List of dofs per entity, ordered by dimension.
  // dof = _entity_dofs[dim][entity][i]
  std::vector<std::vector<std::vector<int>>> _entity_dofs;

  // List of sub dofmaps
  std::vector<std::unique_ptr<ElementDofMap>> sub_dofmaps;
};
} // namespace fem
} // namespace dolfin
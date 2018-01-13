// Copyright (C) 2013 Chris N Richardson
//
// This file is part of DOLFIN (https://www.fenicsproject.org)
//
// SPDX-License-Identifier:    LGPL-3.0-or-later

#ifdef HAS_HDF5

#include "HDF5Attribute.h"
#include "HDF5Interface.h"
#include <boost/lexical_cast.hpp>
#include <string>

using namespace dolfin;

//-----------------------------------------------------------------------------
template <typename T>
const std::string
HDF5Attribute::vector_to_string(const std::vector<T>& vector_value) const
{
  std::string value;
  value = "";
  const std::size_t nlast = vector_value.size() - 1;
  for (std::size_t i = 0; i < nlast; ++i)
    value += boost::lexical_cast<std::string>(vector_value[i]) + ", ";
  value += boost::lexical_cast<std::string>(vector_value[nlast]);
  return value;
}
//-----------------------------------------------------------------------------
template <typename T>
void HDF5Attribute::set_value(const std::string attribute_name,
                              const T& attribute_value)
{
  if (!HDF5Interface::has_dataset(hdf5_file_id, dataset_name))
  {
    dolfin_error("HDF5Attribute.cpp", "set attribute on dataset",
                 "Dataset does not exist");
  }

  if (HDF5Interface::has_attribute(hdf5_file_id, dataset_name, attribute_name))
  {
    HDF5Interface::delete_attribute(hdf5_file_id, dataset_name, attribute_name);
  }

  HDF5Interface::add_attribute(hdf5_file_id, dataset_name, attribute_name,
                               attribute_value);
}
//-----------------------------------------------------------------------------
template <typename T>
void HDF5Attribute::get_value(const std::string attribute_name,
                              T& attribute_value) const
{
  if (!HDF5Interface::has_dataset(hdf5_file_id, dataset_name))
  {
    dolfin_error("HDF5Attribute.cpp", "get attribute of dataset",
                 "Dataset does not exist");
  }

  if (!HDF5Interface::has_attribute(hdf5_file_id, dataset_name, attribute_name))
  {
    dolfin_error("HDF5Attribute.cpp", "get attribute of dataset",
                 "Attribute does not exist");
  }

  HDF5Interface::get_attribute(hdf5_file_id, dataset_name, attribute_name,
                               attribute_value);
}
//-----------------------------------------------------------------------------
void HDF5Attribute::set(const std::string attribute_name, const double value)
{
  set_value(attribute_name, value);
}
//-----------------------------------------------------------------------------
void HDF5Attribute::set(const std::string attribute_name,
                        const std::size_t value)
{
  set_value(attribute_name, value);
}
//-----------------------------------------------------------------------------
void HDF5Attribute::set(const std::string attribute_name,
                        const std::vector<double>& value)
{
  set_value(attribute_name, value);
}
//-----------------------------------------------------------------------------
void HDF5Attribute::set(const std::string attribute_name,
                        const std::vector<std::size_t>& value)
{
  set_value(attribute_name, value);
}
//-----------------------------------------------------------------------------
void HDF5Attribute::set(const std::string attribute_name,
                        const std::string value)
{
  set_value(attribute_name, value);
}
//-----------------------------------------------------------------------------
void HDF5Attribute::get(const std::string attribute_name, double& value) const
{
  get_value(attribute_name, value);
}
//-----------------------------------------------------------------------------
void HDF5Attribute::get(const std::string attribute_name,
                        std::size_t& value) const
{
  get_value(attribute_name, value);
}
//-----------------------------------------------------------------------------
void HDF5Attribute::get(const std::string attribute_name,
                        std::vector<double>& value) const
{
  get_value(attribute_name, value);
}
//-----------------------------------------------------------------------------
void HDF5Attribute::get(const std::string attribute_name,
                        std::vector<std::size_t>& value) const
{
  get_value(attribute_name, value);
}
//-----------------------------------------------------------------------------
void HDF5Attribute::get(const std::string attribute_name,
                        std::string& value) const
{
  const std::string attribute_type = type_str(attribute_name);
  if (attribute_type == "string")
    get_value(attribute_name, value);
  else if (attribute_type == "float")
  {
    double float_value;
    get_value(attribute_name, float_value);
    value = boost::lexical_cast<std::string>(float_value);
  }
  else if (attribute_type == "int")
  {
    std::size_t int_value;
    get_value(attribute_name, int_value);
    value = std::to_string(int_value);
  }
  else if (attribute_type == "vectorfloat")
  {
    std::vector<double> vector_value;
    get_value(attribute_name, vector_value);
    value = vector_to_string(vector_value);
  }
  else if (attribute_type == "vectorint")
  {
    std::vector<std::size_t> vector_value;
    get_value(attribute_name, vector_value);
    value = vector_to_string(vector_value);
  }
  else
    value = "Unsupported";
}
//-----------------------------------------------------------------------------
bool HDF5Attribute::exists(const std::string attribute_name) const
{
  return HDF5Interface::has_attribute(hdf5_file_id, dataset_name,
                                      attribute_name);
}
//-----------------------------------------------------------------------------
const std::string HDF5Attribute::str(const std::string attribute_name) const
{
  std::string str_result;
  get(attribute_name, str_result);
  return str_result;
}
//-----------------------------------------------------------------------------
const std::string HDF5Attribute::str() const
{
  std::string str_result;
  std::vector<std::string> attrs
      = HDF5Interface::list_attributes(hdf5_file_id, dataset_name);
  for (std::vector<std::string>::iterator s = attrs.begin(); s != attrs.end();
       ++s)
  {
    str_result += *s + " ";
  }
  return str_result;
}
//-----------------------------------------------------------------------------
const std::vector<std::string> HDF5Attribute::list_attributes() const
{
  return HDF5Interface::list_attributes(hdf5_file_id, dataset_name);
}
//-----------------------------------------------------------------------------
const std::string
HDF5Attribute::type_str(const std::string attribute_name) const
{
  return HDF5Interface::get_attribute_type(hdf5_file_id, dataset_name,
                                           attribute_name);
}
//-----------------------------------------------------------------------------

#endif

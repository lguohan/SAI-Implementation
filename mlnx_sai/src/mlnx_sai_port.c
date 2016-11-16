/*
 *  Copyright (C) 2014. Mellanox Technologies, Ltd. ALL RIGHTS RESERVED.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 */

#include "sai_windows.h"
#include "sai.h"
#include "mlnx_sai.h"
#include "assert.h"

#undef  __MODULE__
#define __MODULE__ SAI_PORT

static sx_verbosity_level_t LOG_VAR_NAME(__MODULE__) = SX_VERBOSITY_LEVEL_WARNING;
static sai_status_t mlnx_port_tc_get(_In_ const sai_object_id_t port, _Out_ uint8_t *tc);
static sai_status_t mlnx_port_state_set(_In_ const sai_object_key_t      *key,
                                        _In_ const sai_attribute_value_t *value,
                                        void                             *arg);
static sai_status_t mlnx_port_pvid_set(_In_ const sai_object_key_t      *key,
                                       _In_ const sai_attribute_value_t *value,
                                       void                             *arg);
static sai_status_t mlnx_port_default_vlan_prio_set(_In_ const sai_object_key_t      *key,
                                                    _In_ const sai_attribute_value_t *value,
                                                    void                             *arg);
static sai_status_t mlnx_port_ingress_filter_set(_In_ const sai_object_key_t      *key,
                                                 _In_ const sai_attribute_value_t *value,
                                                 void                             *arg);
static sai_status_t mlnx_port_drop_tags_set(_In_ const sai_object_key_t      *key,
                                            _In_ const sai_attribute_value_t *value,
                                            void                             *arg);
static sai_status_t mlnx_port_internal_loopback_set(_In_ const sai_object_key_t      *key,
                                                    _In_ const sai_attribute_value_t *value,
                                                    void                             *arg);
static sai_status_t mlnx_port_fdb_learning_set(_In_ const sai_object_key_t      *key,
                                               _In_ const sai_attribute_value_t *value,
                                               void                             *arg);
static sai_status_t mlnx_port_mtu_set(_In_ const sai_object_key_t      *key,
                                      _In_ const sai_attribute_value_t *value,
                                      void                             *arg);
static sai_status_t mlnx_port_global_flow_ctrl_set(_In_ const sai_object_key_t      *key,
                                                   _In_ const sai_attribute_value_t *value,
                                                   void                             *arg);
static sai_status_t mlnx_port_speed_set(_In_ const sai_object_key_t      *key,
                                        _In_ const sai_attribute_value_t *value,
                                        void                             *arg);
static sai_status_t mlnx_port_auto_negotiation_set(_In_ const sai_object_key_t      *key,
                                                   _In_ const sai_attribute_value_t *value,
                                                   void                             *arg);
static sai_status_t mlnx_port_wred_set(_In_ const sai_object_key_t      *key,
                                       _In_ const sai_attribute_value_t *value,
                                       void                             *arg);
static sai_status_t mlnx_port_type_get(_In_ const sai_object_key_t   *key,
                                       _Inout_ sai_attribute_value_t *value,
                                       _In_ uint32_t                  attr_index,
                                       _Inout_ vendor_cache_t        *cache,
                                       void                          *arg);
static sai_status_t mlnx_port_state_get(_In_ const sai_object_key_t   *key,
                                        _Inout_ sai_attribute_value_t *value,
                                        _In_ uint32_t                  attr_index,
                                        _Inout_ vendor_cache_t        *cache,
                                        void                          *arg);
static sai_status_t mlnx_port_hw_lanes_get(_In_ const sai_object_key_t   *key,
                                           _Inout_ sai_attribute_value_t *value,
                                           _In_ uint32_t                  attr_index,
                                           _Inout_ vendor_cache_t        *cache,
                                           void                          *arg);
static sai_status_t mlnx_port_supported_breakout_get(_In_ const sai_object_key_t   *key,
                                                     _Inout_ sai_attribute_value_t *value,
                                                     _In_ uint32_t                  attr_index,
                                                     _Inout_ vendor_cache_t        *cache,
                                                     void                          *arg);
static sai_status_t mlnx_port_current_breakout_get(_In_ const sai_object_key_t   *key,
                                                   _Inout_ sai_attribute_value_t *value,
                                                   _In_ uint32_t                  attr_index,
                                                   _Inout_ vendor_cache_t        *cache,
                                                   void                          *arg);
static sai_status_t mlnx_port_supported_speed_get(_In_ const sai_object_key_t   *key,
                                                  _Inout_ sai_attribute_value_t *value,
                                                  _In_ uint32_t                  attr_index,
                                                  _Inout_ vendor_cache_t        *cache,
                                                  void                          *arg);
static sai_status_t mlnx_port_number_of_priority_groups_get(_In_ const sai_object_key_t   *key,
                                                            _Inout_ sai_attribute_value_t *value,
                                                            _In_ uint32_t                  attr_index,
                                                            _Inout_ vendor_cache_t        *cache,
                                                            void                          *arg);
static sai_status_t mlnx_port_priority_group_list_get(_In_ const sai_object_key_t   *key,
                                                      _Inout_ sai_attribute_value_t *value,
                                                      _In_ uint32_t                  attr_index,
                                                      _Inout_ vendor_cache_t        *cache,
                                                      void                          *arg);
static sai_status_t mlnx_port_speed_get(_In_ const sai_object_key_t   *key,
                                        _Inout_ sai_attribute_value_t *value,
                                        _In_ uint32_t                  attr_index,
                                        _Inout_ vendor_cache_t        *cache,
                                        void                          *arg);
static sai_status_t mlnx_port_duplex_get(_In_ const sai_object_key_t   *key,
                                         _Inout_ sai_attribute_value_t *value,
                                         _In_ uint32_t                  attr_index,
                                         _Inout_ vendor_cache_t        *cache,
                                         void                          *arg);
static sai_status_t mlnx_port_auto_negotiation_get(_In_ const sai_object_key_t   *key,
                                                   _Inout_ sai_attribute_value_t *value,
                                                   _In_ uint32_t                  attr_index,
                                                   _Inout_ vendor_cache_t        *cache,
                                                   void                          *arg);
static sai_status_t mlnx_port_pvid_get(_In_ const sai_object_key_t   *key,
                                       _Inout_ sai_attribute_value_t *value,
                                       _In_ uint32_t                  attr_index,
                                       _Inout_ vendor_cache_t        *cache,
                                       void                          *arg);
static sai_status_t mlnx_port_default_vlan_prio_get(_In_ const sai_object_key_t   *key,
                                                    _Inout_ sai_attribute_value_t *value,
                                                    _In_ uint32_t                  attr_index,
                                                    _Inout_ vendor_cache_t        *cache,
                                                    void                          *arg);
static sai_status_t mlnx_port_ingress_filter_get(_In_ const sai_object_key_t   *key,
                                                 _Inout_ sai_attribute_value_t *value,
                                                 _In_ uint32_t                  attr_index,
                                                 _Inout_ vendor_cache_t        *cache,
                                                 void                          *arg);
static sai_status_t mlnx_port_drop_tags_get(_In_ const sai_object_key_t   *key,
                                            _Inout_ sai_attribute_value_t *value,
                                            _In_ uint32_t                  attr_index,
                                            _Inout_ vendor_cache_t        *cache,
                                            void                          *arg);
static sai_status_t mlnx_port_internal_loopback_get(_In_ const sai_object_key_t   *key,
                                                    _Inout_ sai_attribute_value_t *value,
                                                    _In_ uint32_t                  attr_index,
                                                    _Inout_ vendor_cache_t        *cache,
                                                    void                          *arg);
static sai_status_t mlnx_port_fdb_learning_get(_In_ const sai_object_key_t   *key,
                                               _Inout_ sai_attribute_value_t *value,
                                               _In_ uint32_t                  attr_index,
                                               _Inout_ vendor_cache_t        *cache,
                                               void                          *arg);
static sai_status_t mlnx_port_mtu_get(_In_ const sai_object_key_t   *key,
                                      _Inout_ sai_attribute_value_t *value,
                                      _In_ uint32_t                  attr_index,
                                      _Inout_ vendor_cache_t        *cache,
                                      void                          *arg);
static sai_status_t mlnx_port_global_flow_ctrl_get(_In_ const sai_object_key_t   *key,
                                                   _Inout_ sai_attribute_value_t *value,
                                                   _In_ uint32_t                  attr_index,
                                                   _Inout_ vendor_cache_t        *cache,
                                                   void                          *arg);
static sai_status_t mlnx_port_wred_get(_In_ const sai_object_key_t   *key,
                                       _Inout_ sai_attribute_value_t *value,
                                       _In_ uint32_t                  attr_index,
                                       _Inout_ vendor_cache_t        *cache,
                                       void                          *arg);
static sai_status_t mlnx_port_update_dscp_get(_In_ const sai_object_key_t   *key,
                                              _Inout_ sai_attribute_value_t *value,
                                              _In_ uint32_t                  attr_index,
                                              _Inout_ vendor_cache_t        *cache,
                                              void                          *arg);
static sai_status_t mlnx_port_update_dscp_set(_In_ const sai_object_key_t      *key,
                                              _In_ const sai_attribute_value_t *value,
                                              void                             *arg);
static sai_status_t mlnx_port_qos_default_tc_get(_In_ const sai_object_key_t   *key,
                                                 _Inout_ sai_attribute_value_t *value,
                                                 _In_ uint32_t                  attr_index,
                                                 _Inout_ vendor_cache_t        *cache,
                                                 void                          *arg);
static sai_status_t mlnx_port_qos_default_tc_set(_In_ const sai_object_key_t      *key,
                                                 _In_ const sai_attribute_value_t *value,
                                                 void                             *arg);
static sai_status_t mlnx_port_qos_map_id_get(_In_ const sai_object_key_t   *key,
                                             _Inout_ sai_attribute_value_t *value,
                                             _In_ uint32_t                  attr_index,
                                             _Inout_ vendor_cache_t        *cache,
                                             void                          *arg);
static sai_status_t mlnx_port_qos_map_id_set(_In_ const sai_object_key_t      *key,
                                             _In_ const sai_attribute_value_t *value,
                                             void                             *arg);
static sai_status_t mlnx_port_mirror_session_get(_In_ const sai_object_key_t   *key,
                                                 _Inout_ sai_attribute_value_t *value,
                                                 _In_ uint32_t                  attr_index,
                                                 _Inout_ vendor_cache_t        *cache,
                                                 void                          *arg);
static sai_status_t mlnx_port_mirror_session_set(_In_ const sai_object_key_t      *key,
                                                 _In_ const sai_attribute_value_t *value,
                                                 void                             *arg);
static sai_status_t mlnx_port_samplepacket_session_get(_In_ const sai_object_key_t   *key,
                                                       _Inout_ sai_attribute_value_t *value,
                                                       _In_ uint32_t                  attr_index,
                                                       _Inout_ vendor_cache_t        *cache,
                                                       void                          *arg);
static sai_status_t mlnx_port_samplepacket_session_set(_In_ const sai_object_key_t      *key,
                                                       _In_ const sai_attribute_value_t *value,
                                                       void                             *arg);
static sai_status_t mlnx_port_pfc_control_get(_In_ const sai_object_key_t   *key,
                                              _Inout_ sai_attribute_value_t *value,
                                              _In_ uint32_t                  attr_index,
                                              _Inout_ vendor_cache_t        *cache,
                                              void                          *arg);
static sai_status_t mlnx_port_pfc_control_set(_In_ const sai_object_key_t      *key,
                                              _In_ const sai_attribute_value_t *value,
                                              void                             *arg);
static sai_status_t mlnx_port_queue_num_get(_In_ const sai_object_key_t   *key,
                                            _Inout_ sai_attribute_value_t *value,
                                            _In_ uint32_t                  attr_index,
                                            _Inout_ vendor_cache_t        *cache,
                                            void                          *arg);
static sai_status_t mlnx_port_queue_list_get(_In_ const sai_object_key_t   *key,
                                             _Inout_ sai_attribute_value_t *value,
                                             _In_ uint32_t                  attr_index,
                                             _Inout_ vendor_cache_t        *cache,
                                             void                          *arg);
static sai_status_t mlnx_port_sched_groups_num_get(_In_ const sai_object_key_t   *key,
                                                   _Inout_ sai_attribute_value_t *value,
                                                   _In_ uint32_t                  attr_index,
                                                   _Inout_ vendor_cache_t        *cache,
                                                   void                          *arg);
static sai_status_t mlnx_port_sched_groups_list_get(_In_ const sai_object_key_t   *key,
                                                    _Inout_ sai_attribute_value_t *value,
                                                    _In_ uint32_t                  attr_index,
                                                    _Inout_ vendor_cache_t        *cache,
                                                    void                          *arg);
static sai_status_t mlnx_port_sched_get(_In_ const sai_object_key_t   *key,
                                        _Inout_ sai_attribute_value_t *value,
                                        _In_ uint32_t                  attr_index,
                                        _Inout_ vendor_cache_t        *cache,
                                        void                          *arg);
static sai_status_t mlnx_port_sched_set(_In_ const sai_object_key_t      *key,
                                        _In_ const sai_attribute_value_t *value,
                                        void                             *arg);
static sai_status_t mlnx_port_ingress_buffer_profile_list_get(_In_ const sai_object_key_t   *key,
                                                              _Inout_ sai_attribute_value_t *value,
                                                              _In_ uint32_t                  attr_index,
                                                              _Inout_ vendor_cache_t        *cache,
                                                              void                          *arg);
static sai_status_t mlnx_port_ingress_buffer_profile_list_set(_In_ const sai_object_key_t      *key,
                                                              _In_ const sai_attribute_value_t *value,
                                                              void                             *arg);
static sai_status_t mlnx_port_egress_buffer_profile_list_get(_In_ const sai_object_key_t   *key,
                                                             _Inout_ sai_attribute_value_t *value,
                                                             _In_ uint32_t                  attr_index,
                                                             _Inout_ vendor_cache_t        *cache,
                                                             void                          *arg);
static sai_status_t mlnx_port_egress_buffer_profile_list_set(_In_ const sai_object_key_t      *key,
                                                             _In_ const sai_attribute_value_t *value,
                                                             void                             *arg);
static sai_status_t mlnx_port_storm_control_policer_attr_set(_In_ const sai_object_key_t      *key,
                                                             _In_ const sai_attribute_value_t *value,
                                                             _In_ void                        *arg);
static sai_status_t mlnx_port_storm_control_policer_attr_get(_In_ const sai_object_key_t   *key,
                                                             _Inout_ sai_attribute_value_t *value,
                                                             _In_ uint32_t                  attr_index,
                                                             _Inout_ vendor_cache_t        *cache,
                                                             _In_ void                     *arg);
static const sai_attribute_entry_t        port_attribs[] = {
    { SAI_PORT_ATTR_TYPE, false, false, false, true,
      "Port type", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_PORT_ATTR_OPER_STATUS, false, false, false, true,
      "Port operational status", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_PORT_ATTR_HW_LANE_LIST, true, true, false, true,
      "Port HW lane list", SAI_ATTR_VAL_TYPE_U32LIST },
    { SAI_PORT_ATTR_SUPPORTED_BREAKOUT_MODE, false, false, false, true,
      "Port supported breakout modes", SAI_ATTR_VAL_TYPE_S32LIST },
    { SAI_PORT_ATTR_CURRENT_BREAKOUT_MODE, false, false, false, true,
      "Port current breakout mode", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_PORT_ATTR_SUPPORTED_SPEED, false, false, false, true,
      "Port supported speeds", SAI_ATTR_VAL_TYPE_U32LIST },
    { SAI_PORT_ATTR_NUMBER_OF_PRIORITY_GROUPS, false, false, false, true,
      "Port priority group count", SAI_ATTR_VAL_TYPE_U32},
    { SAI_PORT_ATTR_PRIORITY_GROUP_LIST, false, false, false, true,
      "Port priority groups", SAI_ATTR_VAL_TYPE_OBJLIST},
    { SAI_PORT_ATTR_SPEED, true, true, true, true,
      "Port speed", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_PORT_ATTR_FULL_DUPLEX_MODE, false, false, true, true,
      "Port full duplex", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_PORT_ATTR_AUTO_NEG_MODE, false, false, true, true,
      "Port auto negotiation", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_PORT_ATTR_ADMIN_STATE, false, false, true, true,
      "Port admin state", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_PORT_ATTR_MEDIA_TYPE, false, false, true, true,
      "Port media type", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_PORT_ATTR_PORT_VLAN_ID, false, false, true, true,
      "Port Vlan ID", SAI_ATTR_VAL_TYPE_U16 },
    { SAI_PORT_ATTR_DEFAULT_VLAN_PRIORITY, false, false, true, true,
      "Port default vlan priority", SAI_ATTR_VAL_TYPE_U8 },
    { SAI_PORT_ATTR_INGRESS_FILTERING, false, false, true, true,
      "Port ingress filtering", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_PORT_ATTR_DROP_UNTAGGED, false, false, true, true,
      "Port drop untageed", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_PORT_ATTR_DROP_TAGGED, false, false, true, true,
      "Port drop tageed", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_PORT_ATTR_INTERNAL_LOOPBACK, false, false, true, true,
      "Port internal loopback", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_PORT_ATTR_FDB_LEARNING, false, false, true, true,
      "Port fdb learning", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_PORT_ATTR_UPDATE_DSCP, false, false, true, true,
      "Port update DSCP", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_PORT_ATTR_MTU, false, false, true, true,
      "Port mtu", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_PORT_ATTR_FLOOD_STORM_CONTROL_POLICER_ID, false, false, true, true,
      "Port flood storm control", SAI_ATTR_VAL_TYPE_OID },
    { SAI_PORT_ATTR_BROADCAST_STORM_CONTROL_POLICER_ID, false, false, true, true,
      "Port broadcast storm control", SAI_ATTR_VAL_TYPE_OID },
    { SAI_PORT_ATTR_MULTICAST_STORM_CONTROL_POLICER_ID, false, false, true, true,
      "Port multicast storm control", SAI_ATTR_VAL_TYPE_OID },
    { SAI_PORT_ATTR_GLOBAL_FLOW_CONTROL, false, false, true, true,
      "Port global flow control", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_PORT_ATTR_MAX_LEARNED_ADDRESSES, false, false, true, true,
      "Port max learned addresses", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_PORT_ATTR_FDB_LEARNING_LIMIT_VIOLATION, false, false, true, true,
      "Port fdb learning limit violation", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_PORT_ATTR_INGRESS_ACL_LIST, false, true, true, true,
      "Port bind point for ingress ACL objects", SAI_ATTR_VAL_TYPE_OBJLIST },
    { SAI_PORT_ATTR_EGRESS_ACL_LIST, false, true, true, true,
      "Port bind point for egress ACL objects", SAI_ATTR_VAL_TYPE_OBJLIST },
    { SAI_PORT_ATTR_INGRESS_MIRROR_SESSION, false, false, true, true,
      "Port ingress mirror session", SAI_ATTR_VAL_TYPE_OBJLIST },
    { SAI_PORT_ATTR_EGRESS_MIRROR_SESSION, false, false, true, true,
      "Port egress mirror session", SAI_ATTR_VAL_TYPE_OBJLIST },
    { SAI_PORT_ATTR_INGRESS_SAMPLEPACKET_ENABLE, false, false, true, true,
      "Port ingress samplepacket enable", SAI_ATTR_VAL_TYPE_OID },
    { SAI_PORT_ATTR_EGRESS_SAMPLEPACKET_ENABLE, false, false, true, true,
      "Port egress samplepacket enable", SAI_ATTR_VAL_TYPE_OID },
    { SAI_PORT_ATTR_QOS_DEFAULT_TC, false, false, true, true,
      "Port default tc mapping", SAI_ATTR_VAL_TYPE_OID },
    { SAI_PORT_ATTR_QOS_DOT1P_TO_TC_MAP, false, false, true, true,
      "Port dot1p to tc mapping", SAI_ATTR_VAL_TYPE_OID },
    { SAI_PORT_ATTR_QOS_DOT1P_TO_COLOR_MAP, false, false, true, true,
      "Port dot1p to color mapping", SAI_ATTR_VAL_TYPE_OID },
    { SAI_PORT_ATTR_QOS_DSCP_TO_TC_MAP, false, false, true, true,
      "Port dscp to tc mapping", SAI_ATTR_VAL_TYPE_OID },
    { SAI_PORT_ATTR_QOS_DSCP_TO_COLOR_MAP, false, false, true, true,
      "Port dscp to color mapping", SAI_ATTR_VAL_TYPE_OID },
    { SAI_PORT_ATTR_QOS_TC_AND_COLOR_TO_DOT1P_MAP, false, false, true, true,
      "Port tc & color to dot1p mapping", SAI_ATTR_VAL_TYPE_OID },
    { SAI_PORT_ATTR_QOS_TC_AND_COLOR_TO_DSCP_MAP, false, false, true, true,
      "Port tc & color to dscp mapping", SAI_ATTR_VAL_TYPE_OID },
    { SAI_PORT_ATTR_QOS_TC_TO_QUEUE_MAP, false, false, true, true,
      "Port tc to queue mapping", SAI_ATTR_VAL_TYPE_OID },
    { SAI_PORT_ATTR_QOS_TC_TO_PRIORITY_GROUP_MAP, false, false, true, true,
      "Port tc to pg mapping", SAI_ATTR_VAL_TYPE_OID },
    { SAI_PORT_ATTR_QOS_PFC_PRIORITY_TO_PRIORITY_GROUP_MAP, false, false, true, true,
      "Port pg to pfc prio mapping", SAI_ATTR_VAL_TYPE_OID },
    { SAI_PORT_ATTR_QOS_PFC_PRIORITY_TO_QUEUE_MAP, false, false, true, true,
      "Port received pfc prio to queue mapping", SAI_ATTR_VAL_TYPE_OID },
    { SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL, false, false, true, true,
      "Port pfc prio control bitmap", SAI_ATTR_VAL_TYPE_U8 },
    { SAI_PORT_ATTR_QOS_WRED_PROFILE_ID, false, false, true, true,
      "Port wred profile id", SAI_ATTR_VAL_TYPE_OID},
    { SAI_PORT_ATTR_QOS_SCHEDULER_PROFILE_ID, false, false, true, true,
      "Port scheduler id", SAI_ATTR_VAL_TYPE_OID},
    { SAI_PORT_ATTR_QOS_NUMBER_OF_QUEUES, false, false, false, true,
      "Port queue number", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_PORT_ATTR_QOS_QUEUE_LIST, false, false, false, true,
      "Port queue list", SAI_ATTR_VAL_TYPE_OBJLIST },
    { SAI_PORT_ATTR_QOS_NUMBER_OF_SCHEDULER_GROUPS, false, false, false, true,
      "Port scheduler total groups number", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_PORT_ATTR_QOS_SCHEDULER_GROUP_LIST, false, false, false, true,
      "Port scheduler group list", SAI_ATTR_VAL_TYPE_OBJLIST },
    { SAI_PORT_ATTR_QOS_INGRESS_BUFFER_PROFILE_LIST, false, false, true, true,
      "Port ingress buffer profiles", SAI_ATTR_VAL_TYPE_OBJLIST},
    { SAI_PORT_ATTR_QOS_EGRESS_BUFFER_PROFILE_LIST, false, false, true, true,
      "Port egress buffer profiles", SAI_ATTR_VAL_TYPE_OBJLIST},
    { SAI_PORT_ATTR_POLICER_ID, false, true, true, true,
      "Port policer", SAI_ATTR_VAL_TYPE_OID},
    { END_FUNCTIONALITY_ATTRIBS_ID, false, false, false, false,
      "", SAI_ATTR_VAL_TYPE_UNDETERMINED }
};
static const sai_vendor_attribute_entry_t port_vendor_attribs[] = {
    { SAI_PORT_ATTR_TYPE,
      { false, false, false, true },
      { false, false, false, true },
      mlnx_port_type_get, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_OPER_STATUS,
      { false, false, false, true },
      { false, false, false, true },
      mlnx_port_state_get, (void*)SAI_PORT_ATTR_OPER_STATUS,
      NULL, NULL },
    { SAI_PORT_ATTR_HW_LANE_LIST,
      { true, false, false, true },
      { true, false, false, true },
      mlnx_port_hw_lanes_get, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_SUPPORTED_BREAKOUT_MODE,
      { false, false, false, true },
      { false, false, false, true },
      mlnx_port_supported_breakout_get, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_CURRENT_BREAKOUT_MODE,
      { false, false, false, true },
      { false, false, false, true },
      mlnx_port_current_breakout_get, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_SUPPORTED_SPEED,
      { false, false, false, true },
      { false, false, false, true },
      mlnx_port_supported_speed_get, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_NUMBER_OF_PRIORITY_GROUPS,
      { false, false, false, true },
      { false, false, false, true },
      mlnx_port_number_of_priority_groups_get, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_PRIORITY_GROUP_LIST,
      { false, false, false, true },
      { false, false, false, true },
      mlnx_port_priority_group_list_get, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_SPEED,
      { true, false, true, true },
      { true, false, true, true },
      mlnx_port_speed_get, NULL,
      mlnx_port_speed_set, NULL },
    { SAI_PORT_ATTR_FULL_DUPLEX_MODE,
      { false, false, false, true },
      { false, false, true, true },
      mlnx_port_duplex_get, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_AUTO_NEG_MODE,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_auto_negotiation_get, NULL,
      mlnx_port_auto_negotiation_set, NULL },
    { SAI_PORT_ATTR_ADMIN_STATE,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_state_get, (void*)SAI_PORT_ATTR_ADMIN_STATE,
      mlnx_port_state_set, NULL },
    { SAI_PORT_ATTR_MEDIA_TYPE,
      { false, false, false, false },
      { false, false, true, true },
      NULL, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_PORT_VLAN_ID,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_pvid_get, NULL,
      mlnx_port_pvid_set, NULL },
    { SAI_PORT_ATTR_DEFAULT_VLAN_PRIORITY,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_default_vlan_prio_get, NULL,
      mlnx_port_default_vlan_prio_set, NULL },
    { SAI_PORT_ATTR_INGRESS_FILTERING,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_ingress_filter_get, NULL,
      mlnx_port_ingress_filter_set, NULL },
    { SAI_PORT_ATTR_DROP_UNTAGGED,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_drop_tags_get, (void*)SAI_PORT_ATTR_DROP_UNTAGGED,
      mlnx_port_drop_tags_set, (void*)SAI_PORT_ATTR_DROP_UNTAGGED },
    { SAI_PORT_ATTR_DROP_TAGGED,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_drop_tags_get, (void*)SAI_PORT_ATTR_DROP_TAGGED,
      mlnx_port_drop_tags_set, (void*)SAI_PORT_ATTR_DROP_TAGGED },
    { SAI_PORT_ATTR_INTERNAL_LOOPBACK,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_internal_loopback_get, NULL,
      mlnx_port_internal_loopback_set, NULL },
    { SAI_PORT_ATTR_FDB_LEARNING,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_fdb_learning_get, NULL,
      mlnx_port_fdb_learning_set, NULL },
    { SAI_PORT_ATTR_UPDATE_DSCP,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_update_dscp_get, NULL,
      mlnx_port_update_dscp_set, NULL },
    { SAI_PORT_ATTR_MTU,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_mtu_get, NULL,
      mlnx_port_mtu_set, NULL },
    { SAI_PORT_ATTR_FLOOD_STORM_CONTROL_POLICER_ID,
      { false, false, true, true},
      { false, false, true, true },
      mlnx_port_storm_control_policer_attr_get, (void*)MLNX_PORT_POLICER_TYPE_FLOOD_INDEX,
      mlnx_port_storm_control_policer_attr_set, (void*)MLNX_PORT_POLICER_TYPE_FLOOD_INDEX },
    { SAI_PORT_ATTR_BROADCAST_STORM_CONTROL_POLICER_ID,
      { false, false, true, true},
      { false, false, true, true },
      mlnx_port_storm_control_policer_attr_get, (void*)MLNX_PORT_POLICER_TYPE_BROADCAST_INDEX,
      mlnx_port_storm_control_policer_attr_set, (void*)MLNX_PORT_POLICER_TYPE_BROADCAST_INDEX },
    { SAI_PORT_ATTR_MULTICAST_STORM_CONTROL_POLICER_ID,
      { false, false, true, true},
      { false, false, true, true },
      mlnx_port_storm_control_policer_attr_get, (void*)MLNX_PORT_POLICER_TYPE_MULTICAST_INDEX,
      mlnx_port_storm_control_policer_attr_set, (void*)MLNX_PORT_POLICER_TYPE_MULTICAST_INDEX },
    { SAI_PORT_ATTR_GLOBAL_FLOW_CONTROL,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_global_flow_ctrl_get, NULL,
      mlnx_port_global_flow_ctrl_set, NULL },
    { SAI_PORT_ATTR_MAX_LEARNED_ADDRESSES,
      { false, false, false, false },
      { false, false, true, true },
      NULL, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_FDB_LEARNING_LIMIT_VIOLATION,
      { false, false, false, false },
      { false, false, true, true },
      NULL, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_INGRESS_ACL_LIST,
      { true, false, true, true },
      { true, false, true, true },
      mlnx_acl_bind_point_get, (void*)MLNX_ACL_BIND_POINT_TYPE_INGRESS_PORT,
      mlnx_acl_bind_point_set, (void*)MLNX_ACL_BIND_POINT_TYPE_INGRESS_PORT },
    { SAI_PORT_ATTR_EGRESS_ACL_LIST,
      { true, false, true, true },
      { true, false, true, true },
      mlnx_acl_bind_point_get, (void*)MLNX_ACL_BIND_POINT_TYPE_EGRESS_PORT,
      mlnx_acl_bind_point_set, (void*)MLNX_ACL_BIND_POINT_TYPE_EGRESS_PORT },
    { SAI_PORT_ATTR_INGRESS_MIRROR_SESSION,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_mirror_session_get, (void*)MIRROR_INGRESS_PORT,
      mlnx_port_mirror_session_set, (void*)MIRROR_INGRESS_PORT },
    { SAI_PORT_ATTR_EGRESS_MIRROR_SESSION,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_mirror_session_get, (void*)MIRROR_EGRESS_PORT,
      mlnx_port_mirror_session_set, (void*)MIRROR_EGRESS_PORT },
    { SAI_PORT_ATTR_INGRESS_SAMPLEPACKET_ENABLE,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_samplepacket_session_get, (void*)SAMPLEPACKET_INGRESS_PORT,
      mlnx_port_samplepacket_session_set, (void*)SAMPLEPACKET_INGRESS_PORT },
    { SAI_PORT_ATTR_EGRESS_SAMPLEPACKET_ENABLE,
      { false, false, false, false },
      { false, false, true, true },
      mlnx_port_samplepacket_session_get, (void*)SAMPLEPACKET_EGRESS_PORT,
      mlnx_port_samplepacket_session_set, (void*)SAMPLEPACKET_EGRESS_PORT },
    { SAI_PORT_ATTR_QOS_DEFAULT_TC,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_qos_default_tc_get, NULL,
      mlnx_port_qos_default_tc_set, NULL },
    { SAI_PORT_ATTR_QOS_DOT1P_TO_TC_MAP,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_qos_map_id_get, (void*)SAI_QOS_MAP_DOT1P_TO_TC,
      mlnx_port_qos_map_id_set, (void*)SAI_QOS_MAP_DOT1P_TO_TC },
    { SAI_PORT_ATTR_QOS_DOT1P_TO_COLOR_MAP,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_qos_map_id_get, (void*)SAI_QOS_MAP_DOT1P_TO_COLOR,
      mlnx_port_qos_map_id_set, (void*)SAI_QOS_MAP_DOT1P_TO_COLOR },
    { SAI_PORT_ATTR_QOS_DSCP_TO_TC_MAP,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_qos_map_id_get, (void*)SAI_QOS_MAP_DSCP_TO_TC,
      mlnx_port_qos_map_id_set, (void*)SAI_QOS_MAP_DSCP_TO_TC },
    { SAI_PORT_ATTR_QOS_DSCP_TO_COLOR_MAP,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_qos_map_id_get, (void*)SAI_QOS_MAP_DSCP_TO_COLOR,
      mlnx_port_qos_map_id_set, (void*)SAI_QOS_MAP_DSCP_TO_COLOR },
    { SAI_PORT_ATTR_QOS_TC_AND_COLOR_TO_DOT1P_MAP,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_qos_map_id_get, (void*)SAI_QOS_MAP_TC_AND_COLOR_TO_DOT1P,
      mlnx_port_qos_map_id_set, (void*)SAI_QOS_MAP_TC_AND_COLOR_TO_DOT1P },
    { SAI_PORT_ATTR_QOS_TC_AND_COLOR_TO_DSCP_MAP,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_qos_map_id_get, (void*)SAI_QOS_MAP_TC_AND_COLOR_TO_DSCP,
      mlnx_port_qos_map_id_set, (void*)SAI_QOS_MAP_TC_AND_COLOR_TO_DSCP },
    { SAI_PORT_ATTR_QOS_TC_TO_QUEUE_MAP,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_qos_map_id_get, (void*)SAI_QOS_MAP_TC_TO_QUEUE,
      mlnx_port_qos_map_id_set, (void*)SAI_QOS_MAP_TC_TO_QUEUE },
    { SAI_PORT_ATTR_QOS_TC_TO_PRIORITY_GROUP_MAP,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_qos_map_id_get, (void*)SAI_QOS_MAP_TC_TO_PRIORITY_GROUP,
      mlnx_port_qos_map_id_set, (void*)SAI_QOS_MAP_TC_TO_PRIORITY_GROUP },
    { SAI_PORT_ATTR_QOS_PFC_PRIORITY_TO_PRIORITY_GROUP_MAP,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_qos_map_id_get, (void*)SAI_QOS_MAP_PFC_PRIORITY_TO_PRIORITY_GROUP,
      mlnx_port_qos_map_id_set, (void*)SAI_QOS_MAP_PFC_PRIORITY_TO_PRIORITY_GROUP },
    { SAI_PORT_ATTR_QOS_PFC_PRIORITY_TO_QUEUE_MAP,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_qos_map_id_get, (void*)SAI_QOS_MAP_PFC_PRIORITY_TO_QUEUE,
      mlnx_port_qos_map_id_set, (void*)SAI_QOS_MAP_PFC_PRIORITY_TO_QUEUE },
    { SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_pfc_control_get, NULL,
      mlnx_port_pfc_control_set, NULL },
    { SAI_PORT_ATTR_QOS_WRED_PROFILE_ID,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_wred_get, NULL,
      mlnx_port_wred_set, NULL },
    { SAI_PORT_ATTR_QOS_SCHEDULER_PROFILE_ID,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_sched_get, NULL,
      mlnx_port_sched_set, NULL },
    { SAI_PORT_ATTR_QOS_NUMBER_OF_QUEUES,
      { false, false, false, true },
      { false, false, false, true },
      mlnx_port_queue_num_get, NULL,
      NULL, NULL},
    { SAI_PORT_ATTR_QOS_QUEUE_LIST,
      { false, false, false, true },
      { false, false, false, true },
      mlnx_port_queue_list_get, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_QOS_NUMBER_OF_SCHEDULER_GROUPS,
      { false, false, false, true },
      { false, false, false, true },
      mlnx_port_sched_groups_num_get, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_QOS_SCHEDULER_GROUP_LIST,
      { false, false, false, true },
      { false, false, false, true },
      mlnx_port_sched_groups_list_get, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_QOS_INGRESS_BUFFER_PROFILE_LIST,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_ingress_buffer_profile_list_get, NULL,
      mlnx_port_ingress_buffer_profile_list_set, NULL },

    { SAI_PORT_ATTR_QOS_EGRESS_BUFFER_PROFILE_LIST,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_egress_buffer_profile_list_get, NULL,
      mlnx_port_egress_buffer_profile_list_set, NULL },
    { SAI_PORT_ATTR_POLICER_ID,
      { false, false, true, true },
      { false, false, true, true },
      mlnx_port_storm_control_policer_attr_get, (void*)MLNX_PORT_POLICER_TYPE_REGULAR_INDEX,
      mlnx_port_storm_control_policer_attr_set, (void*)MLNX_PORT_POLICER_TYPE_REGULAR_INDEX },
};
static sai_status_t check_attrs_port_type(_In_ const sai_object_key_t *key,
                                          _In_ uint32_t                count,
                                          _In_ const sai_attribute_t  *attrs)
{
    uint32_t ii;

    sai_db_read_lock();
    for (ii = 0; ii < count; ii++) {
        attr_port_type_check_t check = ATTR_PORT_IS_ENABLED | ATTR_PORT_IS_IN_LAG_ENABLED;
        const sai_attribute_t *attr  = &attrs[ii];
        sai_status_t           status;

        switch (attr->id) {
        /* LAG members not supported */
        case SAI_PORT_ATTR_PORT_VLAN_ID:
        case SAI_PORT_ATTR_DEFAULT_VLAN_PRIORITY:
        case SAI_PORT_ATTR_INGRESS_FILTERING:
        case SAI_PORT_ATTR_DROP_UNTAGGED:
        case SAI_PORT_ATTR_DROP_TAGGED:
        case SAI_PORT_ATTR_FDB_LEARNING:
        case SAI_PORT_ATTR_MAX_LEARNED_ADDRESSES:
        case SAI_PORT_ATTR_FDB_LEARNING_LIMIT_VIOLATION:
            check = ATTR_PORT_IS_LAG_ENABLED;
            break;
        }

        status = check_port_type_attr(&key->object_id, 1, check, attr->id, ii);
        if (SAI_ERR(status)) {
            sai_db_unlock();
            return status;
        }
    }
    sai_db_unlock();

    return SAI_STATUS_SUCCESS;
}


/* Admin Mode [bool] */
static sai_status_t mlnx_port_state_set(_In_ const sai_object_key_t      *key,
                                        _In_ const sai_attribute_value_t *value,
                                        void                             *arg)
{
    sai_status_t     status;
    sx_port_log_id_t port_id;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL))) {
        return status;
    }

    if (SX_STATUS_SUCCESS != (status = sx_api_port_state_set(gh_sdk, port_id,
                                                             value->booldata ? SX_PORT_ADMIN_STATUS_UP :
                                                             SX_PORT_ADMIN_STATUS_DOWN))) {
        SX_LOG_ERR("Failed to set port admin state - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Port VLAN ID [sai_vlan_id_t]
 * Untagged ingress frames are tagged with Port VLAN ID (PVID)
 */
static sai_status_t mlnx_port_pvid_set(_In_ const sai_object_key_t      *key,
                                       _In_ const sai_attribute_value_t *value,
                                       void                             *arg)
{
    sai_status_t     status;
    sx_port_log_id_t port_id;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_log_port(key->object_id, &port_id))) {
        return status;
    }

    if (SX_STATUS_SUCCESS !=
        (status = sx_api_vlan_port_pvid_set(gh_sdk, SX_ACCESS_CMD_ADD, port_id, value->u16))) {
        SX_LOG_ERR("Failed to set port pvid - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Default VLAN Priority [uint8_t]
 *  (default to 0) */
static sai_status_t mlnx_port_default_vlan_prio_set(_In_ const sai_object_key_t      *key,
                                                    _In_ const sai_attribute_value_t *value,
                                                    void                             *arg)
{
    sai_status_t     status;
    sx_port_log_id_t port_id;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_log_port(key->object_id, &port_id))) {
        return status;
    }

    if (SX_STATUS_SUCCESS !=
        (status = sx_api_cos_port_default_prio_set(gh_sdk, port_id, value->u8))) {
        SX_LOG_ERR("Failed to set port default prio - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Ingress Filtering (Drop Frames with Unknown VLANs) [bool] */
static sai_status_t mlnx_port_ingress_filter_set(_In_ const sai_object_key_t      *key,
                                                 _In_ const sai_attribute_value_t *value,
                                                 void                             *arg)
{
    sai_status_t     status;
    sx_port_log_id_t port_id;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_log_port(key->object_id, &port_id))) {
        return status;
    }

    if (SX_STATUS_SUCCESS != (status = sx_api_vlan_port_ingr_filter_set(gh_sdk, port_id,
                                                                        value->booldata ? SX_INGR_FILTER_ENABLE :
                                                                        SX_INGR_FILTER_DISABLE))) {
        SX_LOG_ERR("Failed to set port ingress filter - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Dropping of untagged frames on ingress [bool] */
/* Dropping of tagged frames on ingress [bool] */
static sai_status_t mlnx_port_drop_tags_set(_In_ const sai_object_key_t      *key,
                                            _In_ const sai_attribute_value_t *value,
                                            void                             *arg)
{
    sai_status_t          status;
    sx_port_log_id_t      port_id;
    sx_vlan_frame_types_t frame_types;

    SX_LOG_ENTER();

    assert((SAI_PORT_ATTR_DROP_UNTAGGED == (long)arg) || (SAI_PORT_ATTR_DROP_TAGGED == (long)arg));

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_log_port(key->object_id, &port_id))) {
        return status;
    }

    if (SX_STATUS_SUCCESS != (status = sx_api_vlan_port_accptd_frm_types_get(gh_sdk, port_id, &frame_types))) {
        SX_LOG_ERR("Failed to get port accepted frame types - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    if (SAI_PORT_ATTR_DROP_UNTAGGED == (long)arg) {
        frame_types.allow_untagged = !(value->booldata);
    } else if (SAI_PORT_ATTR_DROP_TAGGED == (long)arg) {
        frame_types.allow_tagged = !(value->booldata);
    }

    if (SX_STATUS_SUCCESS != (status = sx_api_vlan_port_accptd_frm_types_set(gh_sdk, port_id, &frame_types))) {
        SX_LOG_ERR("Failed to set port accepted frame types - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Internal loopback control [sai_port_loopback_mode_t] */
static sai_status_t mlnx_port_internal_loopback_set(_In_ const sai_object_key_t      *key,
                                                    _In_ const sai_attribute_value_t *value,
                                                    void                             *arg)
{
    sai_status_t     status;
    sx_port_log_id_t port_id;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL))) {
        return status;
    }

    /* loopback is currently not implemented for Spectrum */
    /*
     *  sx_port_phys_loopback_t loop_val;
     *  switch (value->s32) {
     *  case SAI_PORT_INTERNAL_LOOPBACK_NONE:
     *   loop_val = SX_PORT_PHYS_LOOPBACK_DISABLE;
     *   break;
     *
     *  case SAI_PORT_INTERNAL_LOOPBACK_PHY:
     *   SX_LOG_ERR("Port internal phy loopback not supported\n");
     *   return SAI_STATUS_ATTR_NOT_SUPPORTED_0;
     *
     *  case SAI_PORT_INTERNAL_LOOPBACK_MAC:
     *   loop_val = SX_PORT_PHYS_LOOPBACK_ENABLE_INTERNAL;
     *   break;
     *
     *  default:
     *   SX_LOG_ERR("Invalid port internal loopback value %d\n", value->s32);
     *   return SAI_STATUS_INVALID_ATTR_VALUE_0;
     *  }
     *
     *  if (SX_STATUS_SUCCESS != (status = sx_api_port_phys_loopback_set(gh_sdk, port_id, loop_val))) {
     *   SX_LOG_ERR("Failed to set port physical loopback - %s.\n", SX_STATUS_MSG(status));
     *   return sdk_to_sai(status);
     *  }*/

    SX_LOG_EXIT();
    return SAI_STATUS_NOT_IMPLEMENTED;
}

/* FDB Learning mode [sai_port_fdb_learning_mode_t] */
static sai_status_t mlnx_port_fdb_learning_set(_In_ const sai_object_key_t      *key,
                                               _In_ const sai_attribute_value_t *value,
                                               void                             *arg)
{
    sai_status_t        status;
    sx_port_log_id_t    port_id;
    sx_fdb_learn_mode_t learn_mode;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_log_port(key->object_id, &port_id))) {
        return status;
    }

    switch (value->s32) {
    case SAI_PORT_LEARN_MODE_DISABLE:
        learn_mode = SX_FDB_LEARN_MODE_DONT_LEARN;
        break;

    case SAI_PORT_LEARN_MODE_HW:
        learn_mode = SX_FDB_LEARN_MODE_AUTO_LEARN;
        break;

    case SAI_PORT_LEARN_MODE_CPU_LOG:
        learn_mode = SX_FDB_LEARN_MODE_CONTROL_LEARN;
        break;

    case SAI_PORT_LEARN_MODE_DROP:
    case SAI_PORT_LEARN_MODE_CPU_TRAP:
        return SAI_STATUS_NOT_IMPLEMENTED;

    default:
        SX_LOG_ERR("Invalid port fdb learning mode %d\n", value->s32);
        return SAI_STATUS_INVALID_ATTR_VALUE_0;
    }

    if (SX_STATUS_SUCCESS != (status = sx_api_fdb_port_learn_mode_set(gh_sdk, port_id, learn_mode))) {
        SX_LOG_ERR("Failed to set port learning mode - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* MTU [uint32_t] */
static sai_status_t mlnx_port_mtu_set(_In_ const sai_object_key_t      *key,
                                      _In_ const sai_attribute_value_t *value,
                                      void                             *arg)
{
    sai_status_t     status;
    sx_port_log_id_t port_id;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL))) {
        return status;
    }

    if (SX_STATUS_SUCCESS != (status = sx_api_port_mtu_set(gh_sdk, port_id, (sx_port_mtu_t)value->u32))) {
        SX_LOG_ERR("Failed to set port mtu - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/** [sai_port_flow_control_mode_t]
 *  (default to SAI_PORT_FLOW_CONTROL_DISABLE) */
static sai_status_t mlnx_port_global_flow_ctrl_set(_In_ const sai_object_key_t      *key,
                                                   _In_ const sai_attribute_value_t *value,
                                                   void                             *arg)
{
    sx_port_log_id_t         port_id;
    sai_status_t             status;
    sx_port_flow_ctrl_mode_t ctrl_mode = SX_PORT_FLOW_CTRL_MODE_TX_DIS_RX_DIS;

    SX_LOG_ENTER();

    status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL);
    if (SAI_ERR(status)) {
        goto out;
    }

    switch (value->u8) {
    case SAI_PORT_FLOW_CONTROL_DISABLE:
        ctrl_mode = SX_PORT_FLOW_CTRL_MODE_TX_DIS_RX_DIS;
        break;

    case SAI_PORT_FLOW_CONTROL_TX_ONLY:
        ctrl_mode = SX_PORT_FLOW_CTRL_MODE_TX_EN_RX_DIS;
        break;

    case SAI_PORT_FLOW_CONTROL_RX_ONLY:
        ctrl_mode = SX_PORT_FLOW_CTRL_MODE_TX_DIS_RX_EN;
        break;

    case SAI_PORT_FLOW_CONTROL_BOTH_ENABLE:
        ctrl_mode = SX_PORT_FLOW_CTRL_MODE_TX_EN_RX_EN;
        break;

    default:
        SX_LOG_ERR("Invalid SAI global flow control mode %u\n", ctrl_mode);
        status = SAI_STATUS_INVALID_PARAMETER;
        goto out;
    }

    status = sx_api_port_global_fc_enable_set(gh_sdk, port_id, ctrl_mode);
    if (status != SX_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to set port global flow control - %s\n", SX_STATUS_MSG(status));
        status = sdk_to_sai(status);
        goto out;
    }

out:
    SX_LOG_EXIT();
    return status;
}

static sai_status_t port_speed_set(sx_port_log_id_t port_log_id, uint32_t value)
{
    sx_status_t                status;
    sx_port_speed_capability_t speed;

    memset(&speed, 0, sizeof(speed));

    /* Use values for copper cables, which are the default media type. TODO : support additional media types */
    switch (value) {
    case PORT_SPEED_1:
        speed.mode_1GB_CX_SGMII = true;
        speed.mode_1GB_KX       = true;
        break;

    case PORT_SPEED_10:
        speed.mode_10GB_CX4_XAUI = true;
        speed.mode_10GB_KR       = true;
        speed.mode_10GB_CR       = true;
        speed.mode_10GB_SR       = true;
        speed.mode_10GB_ER_LR    = true;
        speed.mode_10GB_KX4      = true;
        break;

    case PORT_SPEED_20:
        speed.mode_20GB_KR2 = true;
        break;

    case PORT_SPEED_40:
        speed.mode_40GB_CR4     = true;
        speed.mode_40GB_SR4     = true;
        speed.mode_40GB_LR4_ER4 = true;
        speed.mode_40GB_KR4     = true;
        break;

    case PORT_SPEED_56:
        speed.mode_56GB_KR4 = true;
        speed.mode_56GB_KX4 = true;
        break;

    case PORT_SPEED_100:
        speed.mode_100GB_CR4     = true;
        speed.mode_100GB_SR4     = true;
        speed.mode_100GB_LR4_ER4 = true;
        speed.mode_100GB_KR4     = true;
        break;

    case PORT_SPEED_50:
        speed.mode_50GB_CR2 = true;
        speed.mode_50GB_KR2 = true;
        break;

    case PORT_SPEED_25:
        speed.mode_25GB_CR = true;
        speed.mode_25GB_SR = true;
        speed.mode_25GB_KR = true;
        break;

    default:
        SX_LOG_ERR("Invalid speed %u\n", value);
        return SAI_STATUS_INVALID_ATTR_VALUE_0;
    }

    status = sx_api_port_speed_admin_set(gh_sdk, port_log_id, &speed);
    if (SX_ERR(status)) {
        SX_LOG_ERR("Failed to set port speed - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    return SAI_STATUS_SUCCESS;
}

/* Speed in Mbps [uint32_t] */
static sai_status_t mlnx_port_speed_set(_In_ const sai_object_key_t      *key,
                                        _In_ const sai_attribute_value_t *value,
                                        void                             *arg)
{
    sai_status_t     status;
    sx_port_log_id_t port_id;

    SX_LOG_ENTER();

    status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL);
    if (SAI_ERR(status)) {
        SX_LOG_EXIT();
        return status;
    }

    status = port_speed_set(port_id, value->u32);

    SX_LOG_EXIT();
    return status;
}

/* Auto Negotiation configuration [bool] */
static sai_status_t mlnx_port_auto_negotiation_set(_In_ const sai_object_key_t      *key,
                                                   _In_ const sai_attribute_value_t *value,
                                                   void                             *arg)
{
    sai_status_t               status;
    sx_port_log_id_t           port_id;
    sx_port_speed_capability_t speed;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL))) {
        return status;
    }

    memset(&speed, 0, sizeof(speed));

    speed.mode_auto = value->booldata;

    if (SX_STATUS_SUCCESS != (status = sx_api_port_speed_admin_set(gh_sdk, port_id, &speed))) {
        SX_LOG_ERR("Failed to set port speed - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Set WRED profile to a port */
static sai_status_t mlnx_port_wred_set(_In_ const sai_object_key_t      *key,
                                       _In_ const sai_attribute_value_t *value,
                                       void                             *arg)
{
    sai_object_id_t  wred_id = value->oid;
    sai_status_t     status  = SAI_STATUS_SUCCESS;
    sx_port_log_id_t port_id;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL))) {
        return status;
    }

    status = mlnx_wred_apply(wred_id, key->object_id);

    if (SAI_STATUS_SUCCESS == status) {
        SX_LOG_NTC("Applied WRED profile to port 0%x\n", port_id);
    } else {
        SX_LOG_ERR("Failed to apply WRED profile to port 0%x\n", port_id);
    }

    SX_LOG_EXIT();
    return status;
}

/* Port type [sai_port_type_t] */
static sai_status_t mlnx_port_type_get(_In_ const sai_object_key_t   *key,
                                       _Inout_ sai_attribute_value_t *value,
                                       _In_ uint32_t                  attr_index,
                                       _Inout_ vendor_cache_t        *cache,
                                       void                          *arg)
{
    sai_status_t     status;
    sx_port_log_id_t port_id;
    sx_port_mode_t   port_mode;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL))) {
        return status;
    }

    if (SX_STATUS_SUCCESS != (status = sx_api_port_mode_get(gh_sdk, port_id, &port_mode))) {
        SX_LOG_ERR("Failed to get port mode - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    switch (port_mode) {
    case SX_PORT_MODE_EXTERNAL:
        value->s32 = SAI_PORT_TYPE_LOGICAL;
        break;

    case SX_PORT_MODE_CPU:
        value->s32 = SAI_PORT_TYPE_CPU;
        break;

    /* TODO : add case for LAG */

    case SX_PORT_MODE_STACKING:
    case SX_PORT_MODE_TCA_CONNECTOR:
    default:
        SX_LOG_ERR("Unexpected port mode %d\n", port_mode);
        return SAI_STATUS_FAILURE;
    }

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Operational Status [sai_port_oper_status_t] */
/* Admin Mode [bool] */
static sai_status_t mlnx_port_state_get(_In_ const sai_object_key_t   *key,
                                        _Inout_ sai_attribute_value_t *value,
                                        _In_ uint32_t                  attr_index,
                                        _Inout_ vendor_cache_t        *cache,
                                        void                          *arg)
{
    sai_status_t           status;
    sx_port_log_id_t       port_id;
    sx_port_oper_state_t   state_oper;
    sx_port_admin_state_t  state_admin;
    sx_port_module_state_t state_module;

    SX_LOG_ENTER();

    assert((SAI_PORT_ATTR_OPER_STATUS == (long)arg) || (SAI_PORT_ATTR_ADMIN_STATE == (long)arg));

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL))) {
        return status;
    }

    if (SX_STATUS_SUCCESS !=
        (status = sx_api_port_state_get(gh_sdk, port_id, &state_oper, &state_admin, &state_module))) {
        SX_LOG_ERR("Failed to get port state - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    if (SAI_PORT_ATTR_OPER_STATUS == (long)arg) {
        switch (state_oper) {
        case SX_PORT_OPER_STATUS_UP:
            value->s32 = SAI_PORT_OPER_STATUS_UP;
            break;

        case SX_PORT_OPER_STATUS_DOWN:
        case SX_PORT_OPER_STATUS_DOWN_BY_FAIL:
            value->s32 = SAI_PORT_OPER_STATUS_DOWN;
            break;

        default:
            value->s32 = SAI_PORT_OPER_STATUS_UNKNOWN;
        }
    } else {
        switch (state_admin) {
        case SX_PORT_ADMIN_STATUS_UP:
            value->booldata = true;
            break;

        case SX_PORT_ADMIN_STATUS_DOWN:
            value->booldata = false;
            break;

        default:
            SX_LOG_ERR("Unexpected port admin state %d\n", state_admin);
            return SAI_STATUS_FAILURE;
        }
    }

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

_Success_(return == SAI_STATUS_SUCCESS)
sai_status_t find_port_in_db(_In_ sai_object_id_t port, _Out_ uint32_t *index)
{
    mlnx_port_config_t *port_cfg;
    uint32_t            ii;

    if (NULL == index) {
        return SAI_STATUS_INVALID_PARAMETER;
    }

    mlnx_port_foreach(port_cfg, ii) {
        if (port == port_cfg->saiport) {
            *index = ii;
            return SAI_STATUS_SUCCESS;
        }
    }

    SX_LOG_ERR("Port %" PRIx64 " not found in DB\n", port);
    return SAI_STATUS_INVALID_PORT_NUMBER;
}

/* Hardware Lane list [sai_u32_list_t] */
static sai_status_t mlnx_port_hw_lanes_get(_In_ const sai_object_key_t   *key,
                                           _Inout_ sai_attribute_value_t *value,
                                           _In_ uint32_t                  attr_index,
                                           _Inout_ vendor_cache_t        *cache,
                                           void                          *arg)
{
    sx_port_mapping_t port_map;
    sx_port_log_id_t  port_id;
    uint32_t          lanes[4];
    sai_status_t      status;
    uint32_t          ii;

    SX_LOG_ENTER();

    status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL);
    if (SAI_ERR(status)) {
        return status;
    }

    status = sx_api_port_mapping_get(gh_sdk, &port_id, &port_map, 1);
    if (SX_ERR(status)) {
        return sdk_to_sai(status);
    }

    memset(lanes, 0, sizeof(lanes));

    for (ii = 0; ii < port_map.width; ii++) {
        if (port_map.lane_bmap & (1 << ii)) {
            lanes[ii] = port_map.module_port * MAX_LANES + ii;
        }
    }

    status = mlnx_fill_u32list(lanes, port_map.width, &value->u32list);

    SX_LOG_EXIT();
    return status;
}

/* Breakout mode(s) supported [sai_s32_list_t] */
static sai_status_t mlnx_port_supported_breakout_get(_In_ const sai_object_key_t   *key,
                                                     _Inout_ sai_attribute_value_t *value,
                                                     _In_ uint32_t                  attr_index,
                                                     _Inout_ vendor_cache_t        *cache,
                                                     void                          *arg)
{
    int32_t             modes[SAI_PORT_BREAKOUT_MODE_MAX];
    sx_port_log_id_t    port_log_id;
    uint32_t            modes_num;
    sai_status_t        status;
    mlnx_port_config_t *port;

    SX_LOG_ENTER();

    status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_log_id, NULL);
    if (SAI_ERR(status)) {
        return status;
    }

    sai_db_read_lock();

    status = mlnx_port_by_log_id(port_log_id, &port);
    if (SAI_ERR(status)) {
        sai_db_unlock();
        return status;
    }

    modes[0]  = SAI_PORT_BREAKOUT_MODE_1_LANE;
    modes_num = 1;

    switch (port->breakout_modes) {
    case MLNX_PORT_BREAKOUT_CAPABILITY_NONE:
        break;

    case MLNX_PORT_BREAKOUT_CAPABILITY_TWO:
        modes[1]  = SAI_PORT_BREAKOUT_MODE_2_LANE;
        modes_num = 2;
        break;

    case MLNX_PORT_BREAKOUT_CAPABILITY_FOUR:
        modes[1]  = SAI_PORT_BREAKOUT_MODE_4_LANE;
        modes_num = 2;
        break;

    case MLNX_PORT_BREAKOUT_CAPABILITY_TWO_FOUR:
        modes[1]  = SAI_PORT_BREAKOUT_MODE_2_LANE;
        modes[2]  = SAI_PORT_BREAKOUT_MODE_4_LANE;
        modes_num = 3;
        break;

    default:
        SX_LOG_ERR("Invalid breakout capability %d port %" PRIx64 "\n",
                   port->breakout_modes, key->object_id);
        sai_db_unlock();
        return SAI_STATUS_FAILURE;
    }

    status = mlnx_fill_s32list(modes, modes_num, &value->s32list);

    sai_db_unlock();

    SX_LOG_EXIT();
    return status;
}

/* Current breakout mode [sai_port_breakout_mode_type_t] */
static sai_status_t mlnx_port_current_breakout_get(_In_ const sai_object_key_t   *key,
                                                   _Inout_ sai_attribute_value_t *value,
                                                   _In_ uint32_t                  attr_index,
                                                   _Inout_ vendor_cache_t        *cache,
                                                   void                          *arg)
{
    SX_LOG_ENTER();

    value->s32 = SAI_PORT_BREAKOUT_MODE_1_LANE;

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Query list of supported port speed in Mbps [sai_u32_list_t] */
static sai_status_t mlnx_port_supported_speed_get(_In_ const sai_object_key_t   *key,
                                                  _Inout_ sai_attribute_value_t *value,
                                                  _In_ uint32_t                  attr_index,
                                                  _Inout_ vendor_cache_t        *cache,
                                                  void                          *arg)
{
    uint32_t             speeds[NUM_SPEEDS];
    uint32_t             speeds_num = 0;
    sai_status_t         status;
    sx_port_log_id_t     port_id;
    sx_port_capability_t speed_cap;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL))) {
        return status;
    }

    if (SX_STATUS_SUCCESS != (status = sx_api_port_capability_get(gh_sdk, port_id, &speed_cap))) {
        SX_LOG_ERR("Failed to get port speed capability - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    if (speed_cap.speed_capability.mode_100GB_CR4 || speed_cap.speed_capability.mode_100GB_SR4 ||
        speed_cap.speed_capability.mode_100GB_KR4 ||
        speed_cap.speed_capability.mode_100GB_LR4_ER4) {
        speeds[speeds_num++] = PORT_SPEED_100;
    }
    if (speed_cap.speed_capability.mode_25GB_CR || speed_cap.speed_capability.mode_25GB_KR ||
        speed_cap.speed_capability.mode_25GB_SR) {
        speeds[speeds_num++] = PORT_SPEED_25;
    }
    if (speed_cap.speed_capability.mode_50GB_CR2 || speed_cap.speed_capability.mode_50GB_KR2) {
        speeds[speeds_num++] = PORT_SPEED_50;
    }
    if (speed_cap.speed_capability.mode_56GB_KX4 || speed_cap.speed_capability.mode_56GB_KR4) {
        speeds[speeds_num++] = PORT_SPEED_56;
    }
    if (speed_cap.speed_capability.mode_40GB_KR4 || speed_cap.speed_capability.mode_40GB_CR4 ||
        speed_cap.speed_capability.mode_40GB_SR4 ||
        speed_cap.speed_capability.mode_40GB_LR4_ER4) {
        speeds[speeds_num++] = PORT_SPEED_40;
    }
    if (speed_cap.speed_capability.mode_20GB_KR2) {
        speeds[speeds_num++] = PORT_SPEED_20;
    }
    if (speed_cap.speed_capability.mode_10GB_KR || speed_cap.speed_capability.mode_10GB_KX4 ||
        speed_cap.speed_capability.mode_10GB_CX4_XAUI ||
        speed_cap.speed_capability.mode_10GB_CR || speed_cap.speed_capability.mode_10GB_SR ||
        speed_cap.speed_capability.mode_10GB_ER_LR) {
        speeds[speeds_num++] = PORT_SPEED_10;
    }
    if (speed_cap.speed_capability.mode_1GB_CX_SGMII || speed_cap.speed_capability.mode_1GB_KX) {
        speeds[speeds_num++] = PORT_SPEED_1;
    }

    status = mlnx_fill_u32list(speeds, speeds_num, &value->u32list);

    SX_LOG_EXIT();
    return status;
}

static sai_status_t mlnx_port_number_of_priority_groups_get(_In_ const sai_object_key_t   *key,
                                                            _Inout_ sai_attribute_value_t *value,
                                                            _In_ uint32_t                  attr_index,
                                                            _Inout_ vendor_cache_t        *cache,
                                                            void                          *arg)
{
    sai_status_t status = SAI_STATUS_SUCCESS;

    SX_LOG_ENTER();
    value->u32 = mlnx_sai_get_buffer_resource_limits()->num_port_pg_buff;
    SX_LOG_EXIT();
    return status;
}

static sai_status_t mlnx_port_priority_group_list_get(_In_ const sai_object_key_t   *key,
                                                      _Inout_ sai_attribute_value_t *value,
                                                      _In_ uint32_t                  attr_index,
                                                      _Inout_ vendor_cache_t        *cache,
                                                      void                          *arg)
{
    sai_status_t     sai_status;
    sai_object_id_t  sai_pg      = SAI_NULL_OBJECT_ID;
    uint8_t          port_pg_ind = 0;
    uint32_t         db_port_index;
    uint8_t          extended_data[EXTENDED_DATA_SIZE];
    sai_object_id_t* sai_pg_array;

    SX_LOG_ENTER();
    memset(extended_data, 0, sizeof(extended_data));
    sai_pg_array = calloc(mlnx_sai_get_buffer_resource_limits()->num_port_pg_buff, sizeof(sai_object_id_t));
    cl_plock_acquire(&g_sai_db_ptr->p_lock);
    sai_status = mlnx_port_idx_by_obj_id(key->object_id, &db_port_index);
    cl_plock_release(&g_sai_db_ptr->p_lock);

    if (SAI_STATUS_SUCCESS != sai_status) {
        free(sai_pg_array);
        SX_LOG_EXIT();
        return sai_status;
    }
    for (port_pg_ind = 0; port_pg_ind < mlnx_sai_get_buffer_resource_limits()->num_port_pg_buff; port_pg_ind++) {
        extended_data[0] = port_pg_ind;
        if (SAI_STATUS_SUCCESS !=
            (sai_status = mlnx_create_object(SAI_OBJECT_TYPE_PRIORITY_GROUP, db_port_index, extended_data, &sai_pg))) {
            free(sai_pg_array);
            SX_LOG_EXIT();
            return sai_status;
        }
        sai_pg_array[port_pg_ind] = sai_pg;
    }
    sai_status = mlnx_fill_objlist(sai_pg_array,
                                   mlnx_sai_get_buffer_resource_limits()->num_port_pg_buff, &value->objlist);
    free(sai_pg_array);
    SX_LOG_EXIT();
    return sai_status;
}

/* Speed in Mbps [uint32_t] */
static sai_status_t mlnx_port_speed_get(_In_ const sai_object_key_t   *key,
                                        _Inout_ sai_attribute_value_t *value,
                                        _In_ uint32_t                  attr_index,
                                        _Inout_ vendor_cache_t        *cache,
                                        void                          *arg)
{
    sai_status_t               status;
    sx_port_log_id_t           port_id;
    sx_port_speed_capability_t speed_cap;
    sx_port_oper_speed_t       speed_oper;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL))) {
        return status;
    }

    if (SX_STATUS_SUCCESS != (status = sx_api_port_speed_get(gh_sdk, port_id, &speed_cap, &speed_oper))) {
        SX_LOG_ERR("Failed to get port speed - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    if (speed_cap.mode_100GB_CR4 || speed_cap.mode_100GB_SR4 || speed_cap.mode_100GB_KR4 ||
        speed_cap.mode_100GB_LR4_ER4) {
        value->u32 = PORT_SPEED_100;
    } else if (speed_cap.mode_56GB_KX4 || speed_cap.mode_56GB_KR4) {
        value->u32 = PORT_SPEED_56;
    } else if (speed_cap.mode_50GB_CR2 || speed_cap.mode_50GB_KR2) {
        value->u32 = PORT_SPEED_50;
    } else if (speed_cap.mode_40GB_KR4 || speed_cap.mode_40GB_CR4 || speed_cap.mode_40GB_SR4 ||
               speed_cap.mode_40GB_LR4_ER4) {
        value->u32 = PORT_SPEED_40;
    } else if (speed_cap.mode_25GB_CR || speed_cap.mode_25GB_KR || speed_cap.mode_25GB_SR) {
        value->u32 = PORT_SPEED_25;
    } else if (speed_cap.mode_20GB_KR2) {
        value->u32 = PORT_SPEED_20;
    } else if (speed_cap.mode_10GB_KR || speed_cap.mode_10GB_KX4 || speed_cap.mode_10GB_CX4_XAUI ||
               speed_cap.mode_10GB_CR || speed_cap.mode_10GB_SR || speed_cap.mode_10GB_ER_LR) {
        value->u32 = PORT_SPEED_10;
    } else if (speed_cap.mode_1GB_CX_SGMII || speed_cap.mode_1GB_KX) {
        value->u32 = PORT_SPEED_1;
    } else if (speed_cap.mode_auto) {
        value->u32 = PORT_SPEED_100;
    } else {
        SX_LOG_ERR("Unexpected port speed\n");
        return SAI_STATUS_FAILURE;
    }

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Full Duplex setting [bool] */
static sai_status_t mlnx_port_duplex_get(_In_ const sai_object_key_t   *key,
                                         _Inout_ sai_attribute_value_t *value,
                                         _In_ uint32_t                  attr_index,
                                         _Inout_ vendor_cache_t        *cache,
                                         void                          *arg)
{
    SX_LOG_ENTER();

    value->booldata = true;

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Auto Negotiation configuration [bool] */
static sai_status_t mlnx_port_auto_negotiation_get(_In_ const sai_object_key_t   *key,
                                                   _Inout_ sai_attribute_value_t *value,
                                                   _In_ uint32_t                  attr_index,
                                                   _Inout_ vendor_cache_t        *cache,
                                                   void                          *arg)
{
    sai_status_t               status;
    sx_port_log_id_t           port_id;
    sx_port_speed_capability_t speed_cap;
    sx_port_oper_speed_t       speed_oper;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL))) {
        return status;
    }

    if (SX_STATUS_SUCCESS != (status = sx_api_port_speed_get(gh_sdk, port_id, &speed_cap, &speed_oper))) {
        SX_LOG_ERR("Failed to get port speed - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    value->booldata = speed_cap.mode_auto;

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Port VLAN ID [sai_vlan_id_t]
 * Untagged ingress frames are tagged with Port VLAN ID (PVID)
 */
static sai_status_t mlnx_port_pvid_get(_In_ const sai_object_key_t   *key,
                                       _Inout_ sai_attribute_value_t *value,
                                       _In_ uint32_t                  attr_index,
                                       _Inout_ vendor_cache_t        *cache,
                                       void                          *arg)
{
    sai_status_t     status;
    sx_port_log_id_t port_id;
    sx_vid_t         pvid;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_log_port(key->object_id, &port_id))) {
        return status;
    }

    if (SX_STATUS_SUCCESS != (status = sx_api_vlan_port_pvid_get(gh_sdk, port_id, &pvid))) {
        SX_LOG_ERR("Failed to get port pvid - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    value->u16 = pvid;

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Default VLAN Priority [uint8_t]
 *  (default to 0) */
static sai_status_t mlnx_port_default_vlan_prio_get(_In_ const sai_object_key_t   *key,
                                                    _Inout_ sai_attribute_value_t *value,
                                                    _In_ uint32_t                  attr_index,
                                                    _Inout_ vendor_cache_t        *cache,
                                                    void                          *arg)
{
    sai_status_t      status;
    sx_port_log_id_t  port_id;
    sx_cos_priority_t prio;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_log_port(key->object_id, &port_id))) {
        return status;
    }

    if (SX_STATUS_SUCCESS != (status = sx_api_cos_port_default_prio_get(gh_sdk, port_id, &prio))) {
        SX_LOG_ERR("Failed to get port default prio - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    value->u8 = prio;

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Ingress Filtering (Drop Frames with Unknown VLANs) [bool] */
static sai_status_t mlnx_port_ingress_filter_get(_In_ const sai_object_key_t   *key,
                                                 _Inout_ sai_attribute_value_t *value,
                                                 _In_ uint32_t                  attr_index,
                                                 _Inout_ vendor_cache_t        *cache,
                                                 void                          *arg)
{
    sai_status_t          status;
    sx_port_log_id_t      port_id;
    sx_ingr_filter_mode_t mode;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_log_port(key->object_id, &port_id))) {
        return status;
    }

    if (SX_STATUS_SUCCESS != (status = sx_api_vlan_port_ingr_filter_get(gh_sdk, port_id, &mode))) {
        SX_LOG_ERR("Failed to get port ingress filter - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    value->booldata = mode;

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Dropping of untagged frames on ingress [bool] */
/* Dropping of tagged frames on ingress [bool] */
static sai_status_t mlnx_port_drop_tags_get(_In_ const sai_object_key_t   *key,
                                            _Inout_ sai_attribute_value_t *value,
                                            _In_ uint32_t                  attr_index,
                                            _Inout_ vendor_cache_t        *cache,
                                            void                          *arg)
{
    sai_status_t          status;
    sx_port_log_id_t      port_id;
    sx_vlan_frame_types_t frame_types;

    SX_LOG_ENTER();

    assert((SAI_PORT_ATTR_DROP_UNTAGGED == (long)arg) || (SAI_PORT_ATTR_DROP_TAGGED == (long)arg));

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_log_port(key->object_id, &port_id))) {
        return status;
    }

    if (SX_STATUS_SUCCESS != (status = sx_api_vlan_port_accptd_frm_types_get(gh_sdk, port_id, &frame_types))) {
        SX_LOG_ERR("Failed to get port accepted frame types - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    if (SAI_PORT_ATTR_DROP_UNTAGGED == (long)arg) {
        value->booldata = !(frame_types.allow_untagged);
    } else {
        value->booldata = !(frame_types.allow_tagged);
    }

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Internal loopback control [sai_port_internal_loopback_mode_t] */
static sai_status_t mlnx_port_internal_loopback_get(_In_ const sai_object_key_t   *key,
                                                    _Inout_ sai_attribute_value_t *value,
                                                    _In_ uint32_t                  attr_index,
                                                    _Inout_ vendor_cache_t        *cache,
                                                    void                          *arg)
{
    sai_status_t            status;
    sx_port_log_id_t        port_id;
    sx_port_phys_loopback_t loopback;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL))) {
        return status;
    }

    if (SX_STATUS_SUCCESS != (status = sx_api_port_phys_loopback_get(gh_sdk, port_id, &loopback))) {
        SX_LOG_ERR("Failed to get port physical loopback - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    /* is internal loopback enabled bool */
    if ((loopback == SX_PORT_PHYS_LOOPBACK_ENABLE_INTERNAL) || (loopback == SX_PORT_PHYS_LOOPBACK_ENABLE_BOTH)) {
        value->s32 = SAI_PORT_INTERNAL_LOOPBACK_MAC;
    } else {
        value->s32 = SAI_PORT_INTERNAL_LOOPBACK_NONE;
    }

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* FDB Learning mode [sai_port_fdb_learning_mode_t] */
static sai_status_t mlnx_port_fdb_learning_get(_In_ const sai_object_key_t   *key,
                                               _Inout_ sai_attribute_value_t *value,
                                               _In_ uint32_t                  attr_index,
                                               _Inout_ vendor_cache_t        *cache,
                                               void                          *arg)
{
    sai_status_t        status;
    sx_port_log_id_t    port_id;
    sx_fdb_learn_mode_t learn_mode;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_log_port(key->object_id, &port_id))) {
        return status;
    }

    if (SX_STATUS_SUCCESS != (status = sx_api_fdb_port_learn_mode_get(gh_sdk, port_id, &learn_mode))) {
        SX_LOG_ERR("Failed to get port learning mode - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    if (SX_FDB_LEARN_MODE_DONT_LEARN == learn_mode) {
        value->s32 = SAI_PORT_LEARN_MODE_DISABLE;
    } else if (SX_FDB_LEARN_MODE_CONTROL_LEARN == learn_mode) {
        value->s32 = SAI_PORT_LEARN_MODE_CPU_LOG;
    } else {
        value->s32 = SAI_PORT_LEARN_MODE_HW;
    }

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* MTU [uint32_t] */
static sai_status_t mlnx_port_mtu_get(_In_ const sai_object_key_t   *key,
                                      _Inout_ sai_attribute_value_t *value,
                                      _In_ uint32_t                  attr_index,
                                      _Inout_ vendor_cache_t        *cache,
                                      void                          *arg)
{
    sai_status_t     status;
    sx_port_log_id_t port_id;
    sx_port_mtu_t    max_mtu;
    sx_port_mtu_t    oper_mtu;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL))) {
        return status;
    }

    if (SX_STATUS_SUCCESS != (status = sx_api_port_mtu_get(gh_sdk, port_id, &max_mtu, &oper_mtu))) {
        SX_LOG_ERR("Failed to get port mtu - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    value->u32 = oper_mtu;

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/** [sai_port_flow_control_mode_t]
 *  (default to SAI_PORT_FLOW_CONTROL_DISABLE) */
static sai_status_t mlnx_port_global_flow_ctrl_get(_In_ const sai_object_key_t   *key,
                                                   _Inout_ sai_attribute_value_t *value,
                                                   _In_ uint32_t                  attr_index,
                                                   _Inout_ vendor_cache_t        *cache,
                                                   void                          *arg)
{
    sx_port_log_id_t         port_id;
    sai_status_t             status;
    sx_port_flow_ctrl_mode_t ctrl_mode;

    SX_LOG_ENTER();

    status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL);
    if (SAI_ERR(status)) {
        goto out;
    }

    status = sx_api_port_global_fc_enable_get(gh_sdk, port_id, &ctrl_mode);
    if (status != SX_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to get port global flow control - %s\n", SX_STATUS_MSG(status));
        status = sdk_to_sai(status);
        goto out;
    }

    switch (ctrl_mode) {
    case SX_PORT_FLOW_CTRL_MODE_TX_DIS_RX_DIS:
        value->u8 = SAI_PORT_FLOW_CONTROL_DISABLE;
        break;

    case SX_PORT_FLOW_CTRL_MODE_TX_EN_RX_DIS:
        value->u8 = SAI_PORT_FLOW_CONTROL_TX_ONLY;
        break;

    case SX_PORT_FLOW_CTRL_MODE_TX_DIS_RX_EN:
        value->u8 = SAI_PORT_FLOW_CONTROL_RX_ONLY;
        break;

    case SX_PORT_FLOW_CTRL_MODE_TX_EN_RX_EN:
        value->u8 = SAI_PORT_FLOW_CONTROL_BOTH_ENABLE;
        break;

    default:
        SX_LOG_ERR("Invalid SDK global flow control mode %u\n", ctrl_mode);
        status = SAI_STATUS_FAILURE;
        goto out;
    }

out:
    SX_LOG_EXIT();
    return status;
}

/* Update DSCP [bool] */
static sai_status_t mlnx_port_update_dscp_get(_In_ const sai_object_key_t   *key,
                                              _Inout_ sai_attribute_value_t *value,
                                              _In_ uint32_t                  attr_index,
                                              _Inout_ vendor_cache_t        *cache,
                                              void                          *arg)
{
    sx_cos_rewrite_enable_t rewrite_enable;
    sx_port_log_id_t        port_id;
    sai_status_t            status;

    SX_LOG_ENTER();

    status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        return status;
    }

    status = sx_api_cos_port_rewrite_enable_get(gh_sdk, port_id, &rewrite_enable);
    if (status != SAI_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to get dscp rewrite enable - %s\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    value->booldata = rewrite_enable.rewrite_dscp;

    SX_LOG_EXIT();
    return status;
}

/* Update DSCP [bool] */
static sai_status_t mlnx_port_update_dscp_set(_In_ const sai_object_key_t      *key,
                                              _In_ const sai_attribute_value_t *value,
                                              void                             *arg)
{
    sx_cos_rewrite_enable_t rewrite_enable;
    sx_port_log_id_t        port_id;
    sai_status_t            status;

    SX_LOG_ENTER();

    status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        return status;
    }

    status = sx_api_cos_port_rewrite_enable_get(gh_sdk, port_id, &rewrite_enable);
    if (status != SAI_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to get dscp rewrite enable - %s\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    rewrite_enable.rewrite_dscp = value->booldata;
    status                      = sx_api_cos_port_rewrite_enable_set(gh_sdk, port_id, rewrite_enable);
    if (status != SAI_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to set dscp rewrite enable - %s\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    SX_LOG_EXIT();
    return status;
}

static sai_status_t mlnx_port_mirror_session_get(_In_ const sai_object_key_t   *key,
                                                 _Inout_ sai_attribute_value_t *value,
                                                 _In_ uint32_t                  attr_index,
                                                 _Inout_ vendor_cache_t        *cache,
                                                 void                          *arg)
{
    sx_port_log_id_t      sdk_mirror_port_id       = 0;
    sai_status_t          status                   = SAI_STATUS_FAILURE;
    sx_span_session_id_t  sdk_mirror_obj_id        = 0;
    sx_mirror_direction_t sdk_mirror_direction     = SX_SPAN_MIRROR_INGRESS;
    sai_object_id_t       sai_mirror_obj_id        = 0;
    const uint32_t        sai_mirror_session_count = 1;
    mlnx_port_config_t   *port;

    SX_LOG_ENTER();

    assert((MIRROR_INGRESS_PORT == (long)arg) || (MIRROR_EGRESS_PORT == (long)arg));

    if (SAI_STATUS_SUCCESS !=
        (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &sdk_mirror_port_id, NULL))) {
        SX_LOG_ERR("Invalid sai mirror port id %" PRIx64 "\n", key->object_id);
        SX_LOG_EXIT();
        return status;
    }

    /* In case if port is LAG member then use LAG logical id */
    sai_db_read_lock();
    status = mlnx_port_by_log_id(sdk_mirror_port_id, &port);
    if (SAI_ERR(status)) {
        sai_db_unlock();
        return status;
    }
    if (mlnx_port_is_lag_member(port)) {
        sdk_mirror_port_id = port->lag_id;
    }
    sai_db_unlock();

    if (MIRROR_INGRESS_PORT == (long)arg) {
        sdk_mirror_direction = SX_SPAN_MIRROR_INGRESS;
    } else if (MIRROR_EGRESS_PORT == (long)arg) {
        sdk_mirror_direction = SX_SPAN_MIRROR_EGRESS;
    }

    if (SX_STATUS_SUCCESS !=
        (status = sx_api_span_mirror_get(gh_sdk, sdk_mirror_port_id, sdk_mirror_direction,
                                         &sdk_mirror_obj_id))) {
        value->objlist.count = 0;
        /* when no session is attached, api returns entry not found */
        if (SX_STATUS_ENTRY_NOT_FOUND == status) {
            status = SAI_STATUS_SUCCESS;
        } else {
            SX_LOG_ERR("Error getting sdk mirror object id from sdk mirror port id %d %s\n",
                       sdk_mirror_port_id,
                       SX_STATUS_MSG(status));
            status = sdk_to_sai(status);
        }
        SX_LOG_EXIT();
        return status;
    }

    if (SAI_STATUS_SUCCESS !=
        (status = mlnx_create_object(SAI_OBJECT_TYPE_MIRROR, sdk_mirror_obj_id, NULL, &sai_mirror_obj_id))) {
        SX_LOG_ERR("Error creating sai mirror obj id from sdk mirror obj id %d\n", sdk_mirror_obj_id);
        SX_LOG_EXIT();
        return status;
    }

    if (SAI_STATUS_SUCCESS !=
        (status = mlnx_fill_objlist(&sai_mirror_obj_id, sai_mirror_session_count, &value->objlist))) {
        SX_LOG_ERR("Error filling object list using sai mirror obj id %" PRId64 "\n", sai_mirror_obj_id);
        SX_LOG_EXIT();
        return status;
    }

    SX_LOG_EXIT();
    return status;
}

static sai_status_t mlnx_port_mirror_session_set(_In_ const sai_object_key_t      *key,
                                                 _In_ const sai_attribute_value_t *value,
                                                 void                             *arg)
{
    uint32_t              sdk_mirror_port_id       = 0;
    sai_status_t          status                   = SAI_STATUS_FAILURE;
    uint32_t              sdk_mirror_obj_id_u32    = 0;
    sx_span_session_id_t  sdk_mirror_obj_id        = 0;
    sx_mirror_direction_t sdk_mirror_direction     = SX_SPAN_MIRROR_INGRESS;
    const uint32_t        sai_mirror_session_count = 1;

    SX_LOG_ENTER();

    assert((MIRROR_INGRESS_PORT == (long)arg) || (MIRROR_EGRESS_PORT == (long)arg));

    if (SAI_STATUS_SUCCESS !=
        (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &sdk_mirror_port_id, NULL))) {
        SX_LOG_ERR("Invalid sai mirror port id %" PRIx64 "\n", key->object_id);
        SX_LOG_EXIT();
        return status;
    }

    if (MIRROR_INGRESS_PORT == (long)arg) {
        sdk_mirror_direction = SX_SPAN_MIRROR_INGRESS;
    } else if (MIRROR_EGRESS_PORT == (long)arg) {
        sdk_mirror_direction = SX_SPAN_MIRROR_EGRESS;
    }

    if (0 == value->objlist.count) {
        if (SAI_STATUS_SUCCESS !=
            (status =
                 sdk_to_sai(sx_api_span_mirror_get(gh_sdk, (sx_port_log_id_t)sdk_mirror_port_id, sdk_mirror_direction,
                                                   &sdk_mirror_obj_id)))) {
            SX_LOG_ERR("Error getting mirror obj id from sdk mirror port id %x\n", sdk_mirror_port_id);
            SX_LOG_EXIT();
            return status;
        }
        if (SAI_STATUS_SUCCESS !=
            (status =
                 sdk_to_sai(sx_api_span_mirror_state_set(gh_sdk, (sx_port_log_id_t)sdk_mirror_port_id,
                                                         sdk_mirror_direction,
                                                         false)))) {
            SX_LOG_ERR("Error setting mirror port state to false on sdk mirror port id %x\n", sdk_mirror_port_id);
            SX_LOG_EXIT();
            return status;
        }
        if (SAI_STATUS_SUCCESS !=
            (status =
                 sdk_to_sai(sx_api_span_mirror_set(gh_sdk, SX_ACCESS_CMD_DELETE, (sx_port_log_id_t)sdk_mirror_port_id,
                                                   sdk_mirror_direction, sdk_mirror_obj_id)))) {
            SX_LOG_ERR("Error deleting sdk mirror port %x for sdk mirror obj id %d\n",
                       sdk_mirror_port_id,
                       sdk_mirror_obj_id);
            SX_LOG_EXIT();
            return status;
        }
        SX_LOG_NTC("Successfully deleted sdk mirror port %x on sdk mirror obj id %d\n",
                   sdk_mirror_port_id,
                   sdk_mirror_obj_id);
        SX_LOG_EXIT();
        return SAI_STATUS_SUCCESS;
    } else if (sai_mirror_session_count == value->objlist.count) {
        if (SAI_STATUS_SUCCESS !=
            (status =
                 mlnx_object_to_type(value->objlist.list[0], SAI_OBJECT_TYPE_MIRROR, &sdk_mirror_obj_id_u32, NULL))) {
            SX_LOG_ERR("Invalid sai mirror obj id %" PRId64 "\n", value->objlist.list[0]);
            SX_LOG_EXIT();
            return status;
        }

        if (SAI_STATUS_SUCCESS !=
            (status =
                 sdk_to_sai(sx_api_span_mirror_set(gh_sdk, SX_ACCESS_CMD_ADD, (sx_port_log_id_t)sdk_mirror_port_id,
                                                   sdk_mirror_direction,
                                                   (sx_span_session_id_t)sdk_mirror_obj_id_u32)))) {
            SX_LOG_ERR("Error setting sdk mirror port id %x on sdk mirror obj id %d\n",
                       sdk_mirror_port_id,
                       sdk_mirror_obj_id_u32);
            SX_LOG_EXIT();
            return status;
        }
        if (SAI_STATUS_SUCCESS !=
            (status =
                 sdk_to_sai(sx_api_span_mirror_state_set(gh_sdk, (sx_port_log_id_t)sdk_mirror_port_id,
                                                         sdk_mirror_direction,
                                                         true)))) {
            SX_LOG_ERR("Error setting mirror port state to true on sdk mirror port id %x\n", sdk_mirror_port_id);
            SX_LOG_EXIT();
            return status;
        }
    } else {
        SX_LOG_ERR("Only one mirror session can be associated to a mirror port\n");
        SX_LOG_EXIT();
        return SAI_STATUS_NOT_IMPLEMENTED;
    }

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

static sai_status_t mlnx_port_samplepacket_session_get(_In_ const sai_object_key_t   *key,
                                                       _Inout_ sai_attribute_value_t *value,
                                                       _In_ uint32_t                  attr_index,
                                                       _Inout_ vendor_cache_t        *cache,
                                                       void                          *arg)
{
    sai_status_t status                        = SAI_STATUS_FAILURE;
    uint32_t     sdk_samplepacket_port_id      = 0;
    uint32_t     sai_port_db_idx               = 0;
    uint32_t     internal_samplepacket_obj_idx = 0;

    SX_LOG_ENTER();

    assert((SAMPLEPACKET_INGRESS_PORT == (long)arg) || (SAMPLEPACKET_EGRESS_PORT == (long)arg));

    if (SAMPLEPACKET_EGRESS_PORT == (long)arg) {
        SX_LOG_ERR("Egress samplepacket on port is not supported yet\n");
        SX_LOG_EXIT();
        return SAI_STATUS_NOT_SUPPORTED;
    }

    assert(SAMPLEPACKET_INGRESS_PORT == (long)arg);

    if (SAI_STATUS_SUCCESS !=
        (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &sdk_samplepacket_port_id, NULL))) {
        SX_LOG_ERR("Invalid sai samplepacket port id %" PRIx64 "\n", key->object_id);
        SX_LOG_EXIT();
        return status;
    }

    assert(NULL != g_sai_db_ptr);

    sai_db_read_lock();

    if (SAI_STATUS_SUCCESS !=
        (status = mlnx_port_idx_by_log_id(sdk_samplepacket_port_id, &sai_port_db_idx))) {
        SX_LOG_ERR("Cannot find sdk port %d in sai port db\n", sdk_samplepacket_port_id);
        goto cleanup;
    }

    if (MLNX_INVALID_SAMPLEPACKET_SESSION ==
        g_sai_db_ptr->ports_db[sai_port_db_idx].internal_ingress_samplepacket_obj_idx) {
        value->oid = SAI_NULL_OBJECT_ID;
        SX_LOG_DBG("sdk samplepacket port id %d does not have associated ingress samplepacket obj id\n",
                   sdk_samplepacket_port_id);
        status = SAI_STATUS_SUCCESS;
        goto cleanup;
    }

    internal_samplepacket_obj_idx = g_sai_db_ptr->ports_db[sai_port_db_idx].internal_ingress_samplepacket_obj_idx;

    if (SAI_STATUS_SUCCESS !=
        (status =
             mlnx_create_object(SAI_OBJECT_TYPE_SAMPLEPACKET, internal_samplepacket_obj_idx, NULL, &value->oid))) {
        SX_LOG_ERR("Error creating sai samplepacket obj id from internal samplepacket obj idx %d\n",
                   internal_samplepacket_obj_idx);
        goto cleanup;
    }

    status = SAI_STATUS_SUCCESS;

cleanup:
    sai_db_unlock();
    SX_LOG_EXIT();
    return status;
}

static sai_status_t mlnx_port_samplepacket_session_set(_In_ const sai_object_key_t      *key,
                                                       _In_ const sai_attribute_value_t *value,
                                                       void                             *arg)
{
    sai_status_t           status                        = SAI_STATUS_FAILURE;
    uint32_t               sdk_samplepacket_port_id      = 0;
    uint32_t               sai_port_db_idx               = 0;
    uint32_t               internal_samplepacket_obj_idx = 0;
    sx_access_cmd_t        sdk_cmd                       = SX_ACCESS_CMD_EDIT;
    sx_port_sflow_params_t sdk_sflow_params;

    memset(&sdk_sflow_params, 0, sizeof(sx_port_sflow_params_t));
    SX_LOG_ENTER();

    assert((SAMPLEPACKET_INGRESS_PORT == (long)arg) || (SAMPLEPACKET_EGRESS_PORT == (long)arg));

    if (SAMPLEPACKET_EGRESS_PORT == (long)arg) {
        SX_LOG_ERR("Egress samplepacket on port is not supported yet\n");
        SX_LOG_EXIT();
        return SAI_STATUS_NOT_SUPPORTED;
    }

    assert(SAMPLEPACKET_INGRESS_PORT == (long)arg);

    if (SAI_STATUS_SUCCESS !=
        (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &sdk_samplepacket_port_id, NULL))) {
        SX_LOG_ERR("Invalid sai samplepacket port id %" PRIx64 "\n", key->object_id);
        SX_LOG_EXIT();
        return status;
    }

    assert(NULL != g_sai_db_ptr);

    sai_db_write_lock();

    if (SAI_STATUS_SUCCESS !=
        (status = mlnx_port_idx_by_log_id(sdk_samplepacket_port_id, &sai_port_db_idx))) {
        SX_LOG_ERR("Cannot find sdk port %d in sai port db\n", sdk_samplepacket_port_id);
        goto cleanup;
    }

    if (SAI_NULL_OBJECT_ID == value->oid) {
        sdk_cmd = SX_ACCESS_CMD_DELETE;
        if (MLNX_INVALID_SAMPLEPACKET_SESSION ==
            g_sai_db_ptr->ports_db[sai_port_db_idx].internal_ingress_samplepacket_obj_idx) {
            SX_LOG_DBG("No internal ingress samplepacket object has been associated to sdk samplepacket port id %d\n",
                       sdk_samplepacket_port_id);
            status = SAI_STATUS_SUCCESS;
            goto cleanup;
        } else if (SAI_STATUS_SUCCESS !=
                   (status =
                        (sdk_to_sai(sx_api_port_sflow_set(gh_sdk, sdk_cmd, sdk_samplepacket_port_id,
                                                          &sdk_sflow_params))))) {
            SX_LOG_ERR("Error disassociating sdk port id %d with internal samplepacket obj idx %d\n",
                       sdk_samplepacket_port_id,
                       g_sai_db_ptr->ports_db[sai_port_db_idx].internal_ingress_samplepacket_obj_idx);
            goto cleanup;
        }
        SX_LOG_DBG("Successfully disassociated sdk port id %d with internal samplepacket obj idx %d\n",
                   sdk_samplepacket_port_id,
                   g_sai_db_ptr->ports_db[sai_port_db_idx].internal_ingress_samplepacket_obj_idx);
        g_sai_db_ptr->ports_db[sai_port_db_idx].internal_ingress_samplepacket_obj_idx =
            MLNX_INVALID_SAMPLEPACKET_SESSION;
        status = SAI_STATUS_SUCCESS;
        goto cleanup;
    } else {
        if (SAI_STATUS_SUCCESS !=
            (status =
                 mlnx_object_to_type(value->oid, SAI_OBJECT_TYPE_SAMPLEPACKET, &internal_samplepacket_obj_idx,
                                     NULL))) {
            SX_LOG_ERR("Invalid sai samplepacket obj idx %" PRIx64 "\n", value->oid);
            goto cleanup;
        }

        if (MLNX_INVALID_SAMPLEPACKET_SESSION ==
            g_sai_db_ptr->ports_db[sai_port_db_idx].internal_ingress_samplepacket_obj_idx) {
            sdk_cmd = SX_ACCESS_CMD_ADD;
        } else {
            sdk_cmd = SX_ACCESS_CMD_EDIT;
        }

        sdk_sflow_params.ratio =
            g_sai_db_ptr->mlnx_samplepacket_session[internal_samplepacket_obj_idx].sai_sample_rate;
        sdk_sflow_params.deviation        = 0;
        sdk_sflow_params.packet_types.uc  = true;
        sdk_sflow_params.packet_types.mc  = true;
        sdk_sflow_params.packet_types.bc  = true;
        sdk_sflow_params.packet_types.uuc = true;
        sdk_sflow_params.packet_types.umc = true;

        if (SAI_STATUS_SUCCESS !=
            (status =
                 (sdk_to_sai(sx_api_port_sflow_set(gh_sdk, sdk_cmd, sdk_samplepacket_port_id, &sdk_sflow_params))))) {
            SX_LOG_ERR("Error associating sdk port id %d with internal samplepacket obj idx %d\n",
                       sdk_samplepacket_port_id,
                       internal_samplepacket_obj_idx);
            goto cleanup;
        }

        g_sai_db_ptr->ports_db[sai_port_db_idx].internal_ingress_samplepacket_obj_idx = internal_samplepacket_obj_idx;

        status = SAI_STATUS_SUCCESS;
        goto cleanup;
    }

cleanup:
    sai_db_unlock();
    SX_LOG_EXIT();
    return status;
}

/** Port default Traffic class Mapping [sai_uint8_t], Default TC=0*/
static sai_status_t mlnx_port_qos_default_tc_get(_In_ const sai_object_key_t   *key,
                                                 _Inout_ sai_attribute_value_t *value,
                                                 _In_ uint32_t                  attr_index,
                                                 _Inout_ vendor_cache_t        *cache,
                                                 void                          *arg)
{
    sai_status_t status;

    SX_LOG_ENTER();

    status = mlnx_port_tc_get(key->object_id, &value->u8);

    SX_LOG_EXIT();
    return status;
}

/** Port default Traffic class Mapping [sai_uint8_t], Default TC=0*/
static sai_status_t mlnx_port_qos_default_tc_set(_In_ const sai_object_key_t      *key,
                                                 _In_ const sai_attribute_value_t *value,
                                                 void                             *arg)
{
    uint8_t             tc = value->u8;
    mlnx_port_config_t *port;
    sai_status_t        status;
    sx_port_log_id_t    log_port;

    SX_LOG_ENTER();

    status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &log_port, NULL);
    if (SAI_ERR(status)) {
        return status;
    }

    if (!SX_CHECK_MAX(value->u8, MAX_PORT_PRIO)) {
        SX_LOG_ERR("Invalid tc(%u)\n", tc);
        return SAI_STATUS_INVALID_PARAMETER;
    }

    sai_db_write_lock();

    if (!tc) {
        tc = g_sai_db_ptr->switch_default_tc;
    }

    status = mlnx_port_by_log_id(log_port, &port);
    if (SAI_ERR(status)) {
        goto out;
    }

    status = mlnx_port_tc_set(port, tc);
    if (status != SAI_STATUS_SUCCESS) {
        goto out;
    }
    port->default_tc = value->u8;

out:
    sai_db_unlock();
    SX_LOG_EXIT();
    return status;
}

/* db read lock is needed */
static sai_status_t db_port_qos_map_id_get(_In_ const sai_object_id_t port_id,
                                           sai_qos_map_type_t         qos_map_type,
                                           sai_object_id_t           *oid)
{
    mlnx_port_config_t *port;
    sai_status_t        status;
    uint32_t            qos_map_id;
    sx_port_log_id_t    log_port;

    status = mlnx_object_to_type(port_id, SAI_OBJECT_TYPE_PORT, &log_port, NULL);
    if (SAI_ERR(status)) {
        return status;
    }

    status = mlnx_port_by_log_id(log_port, &port);
    if (status != SAI_STATUS_SUCCESS) {
        return status;
    }

    qos_map_id = port->qos_maps[qos_map_type];
    if (!qos_map_id) {
        *oid = SAI_NULL_OBJECT_ID;
        return SAI_STATUS_SUCCESS;
    }

    return mlnx_create_object(SAI_OBJECT_TYPE_QOS_MAPS, qos_map_id, NULL, oid);
}

/* db read/write lock is needed */
static sai_status_t db_port_qos_map_id_set(_In_ const sai_object_id_t port_id,
                                           sai_qos_map_type_t         qos_map_type,
                                           sai_object_id_t            oid)
{
    uint32_t            qos_map_id;
    sai_status_t        status;
    mlnx_port_config_t *port;
    sx_port_log_id_t    log_port;

    if (oid == SAI_NULL_OBJECT_ID) {
        qos_map_id = 0;
    } else {
        status = mlnx_object_to_type(oid, SAI_OBJECT_TYPE_QOS_MAPS, &qos_map_id, NULL);
        if (status != SAI_STATUS_SUCCESS) {
            return status;
        }
    }

    status = mlnx_object_to_type(port_id, SAI_OBJECT_TYPE_PORT, &log_port, NULL);
    if (SAI_ERR(status)) {
        return status;
    }

    status = mlnx_port_by_log_id(log_port, &port);
    if (SAI_ERR(status)) {
        return status;
    }

    port->qos_maps[qos_map_type] = qos_map_id;
    return SAI_STATUS_SUCCESS;
}

/** QoS Map Id [sai_object_id_t] */
static sai_status_t mlnx_port_qos_map_id_get(_In_ const sai_object_key_t   *key,
                                             _Inout_ sai_attribute_value_t *value,
                                             _In_ uint32_t                  attr_index,
                                             _Inout_ vendor_cache_t        *cache,
                                             void                          *arg)
{
    sai_qos_map_type_t qos_map_type = (sai_qos_map_type_t)arg;
    sai_status_t       status;
    uint32_t           map_idx;

    assert(qos_map_type < MLNX_QOS_MAP_TYPES_MAX);

    SX_LOG_ENTER();

    sai_db_read_lock();

    status = db_port_qos_map_id_get(key->object_id, qos_map_type, &value->oid);
    if (status != SAI_STATUS_SUCCESS) {
        goto out;
    }

    if (value->oid != SAI_NULL_OBJECT_ID) {
        goto out;
    }

    map_idx = g_sai_db_ptr->switch_qos_maps[qos_map_type];
    if (!map_idx) {
        value->oid = SAI_NULL_OBJECT_ID;
        goto out;
    }

    status = mlnx_create_object(SAI_OBJECT_TYPE_QOS_MAPS, map_idx, NULL, &value->oid);

out:
    sai_db_unlock();
    SX_LOG_EXIT();
    return status;
}

/* db read lock is needed */
static sai_status_t mlnx_port_qos_map_trust_level_set(sx_port_log_id_t port_id, mlnx_qos_map_t *qos_map, bool enabled)
{
    sx_cos_trust_level_t curr_level;
    sx_cos_trust_level_t level;
    sai_status_t         status;

    if ((qos_map->type == SAI_QOS_MAP_DOT1P_TO_TC) ||
        (qos_map->type == SAI_QOS_MAP_DOT1P_TO_COLOR)) {
        level = SX_COS_TRUST_LEVEL_L2;
    } else if ((qos_map->type == SAI_QOS_MAP_DSCP_TO_TC) ||
               (qos_map->type == SAI_QOS_MAP_DSCP_TO_COLOR)) {
        level = SX_COS_TRUST_LEVEL_L3;
    } else {
        assert(false);
    }

    status = sx_api_cos_port_trust_get(gh_sdk, port_id, &curr_level);
    if (status != SX_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to get trust level - %s\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    if (enabled) {
        if (((curr_level == SX_COS_TRUST_LEVEL_L2) || (curr_level == SX_COS_TRUST_LEVEL_L3)) &&
            (curr_level != level)) {
            curr_level = SX_COS_TRUST_LEVEL_BOTH;
        } else if (curr_level == SX_COS_TRUST_LEVEL_PORT) {
            curr_level = level;
        }
    } else {
        mlnx_port_config_t *port_cfg = NULL;

        status = mlnx_port_by_log_id(port_id, &port_cfg);
        if (SAI_ERR(status)) {
            return status;
        }

        /* Don't disable trust level if one of {DOT1P,DSCP}_TO_{TC,COLOR} mapping is set */
        if ((level == SX_COS_TRUST_LEVEL_L2) && (qos_map->type == SAI_QOS_MAP_DOT1P_TO_TC) &&
            (port_cfg->qos_maps[SAI_QOS_MAP_DOT1P_TO_COLOR] != SAI_NULL_OBJECT_ID)) {
            return SAI_STATUS_SUCCESS;
        } else if ((level == SX_COS_TRUST_LEVEL_L2) && (qos_map->type == SAI_QOS_MAP_DOT1P_TO_COLOR) &&
                   (port_cfg->qos_maps[SAI_QOS_MAP_DOT1P_TO_TC] != SAI_NULL_OBJECT_ID)) {
            return SAI_STATUS_SUCCESS;
        } else if ((level == SX_COS_TRUST_LEVEL_L3) && (qos_map->type == SAI_QOS_MAP_DSCP_TO_TC) &&
                   (port_cfg->qos_maps[SAI_QOS_MAP_DSCP_TO_COLOR] != SAI_NULL_OBJECT_ID)) {
            return SAI_STATUS_SUCCESS;
        } else if ((level == SX_COS_TRUST_LEVEL_L3) && (qos_map->type == SAI_QOS_MAP_DSCP_TO_COLOR) &&
                   (port_cfg->qos_maps[SAI_QOS_MAP_DSCP_TO_TC] != SAI_NULL_OBJECT_ID)) {
            return SAI_STATUS_SUCCESS;
        }

        if (curr_level == SX_COS_TRUST_LEVEL_BOTH) {
            if (level == SX_COS_TRUST_LEVEL_L2) {
                curr_level = SX_COS_TRUST_LEVEL_L3;
            } else if (level == SX_COS_TRUST_LEVEL_L3) {
                curr_level = SX_COS_TRUST_LEVEL_L2;
            }
        } else if (curr_level == level) {
            curr_level = SX_COS_TRUST_LEVEL_PORT;
        }
    }

    status = sx_api_cos_port_trust_set(gh_sdk, port_id, curr_level);
    if (status != SX_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to change trust level - %s\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    return SAI_STATUS_SUCCESS;
}

/* db lock is needed */
static sai_status_t mlnx_port_qos_map_assign_dot1p_to_tc_color(sx_port_log_id_t port_id, mlnx_qos_map_t *qos_map)
{
    sx_status_t             status;
    uint32_t                count = (COS_PCP_MAX_NUM + 1) * (COS_DEI_MAX_NUM + 1);
    sx_cos_pcp_dei_t        pcp_dei[(COS_PCP_MAX_NUM + 1) * (COS_DEI_MAX_NUM + 1)];
    sx_cos_priority_color_t prio_color[(COS_PCP_MAX_NUM + 1) * (COS_DEI_MAX_NUM + 1)];
    uint32_t                ii, jj;

    status = sx_api_cos_port_pcpdei_to_prio_get(gh_sdk, port_id, pcp_dei, prio_color, &count);
    if (status != SX_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to get pcp to prio qos map - %s\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    for (ii = 0; ii < qos_map->count; ii++) {
        for (jj = 0; jj < count; jj++) {
            if (qos_map->from.pcp_dei[ii].pcp != pcp_dei[jj].pcp) {
                continue;
            }

            if (qos_map->type == SAI_QOS_MAP_DOT1P_TO_TC) {
                prio_color[jj].priority = qos_map->to.prio_color[ii].priority;
            } else if (qos_map->type == SAI_QOS_MAP_DOT1P_TO_COLOR) {
                prio_color[jj].color = qos_map->to.prio_color[ii].color;
            } else {
                /* We should not reach here but who knows ...*/
                assert(false);
            }
        }
    }

    status = sx_api_cos_port_pcpdei_to_prio_set(gh_sdk, port_id, pcp_dei, prio_color, count);
    if (status != SX_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to set pcp to prio qos map - %s\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    return SAI_STATUS_SUCCESS;
}

static sai_status_t mlnx_port_qos_map_assign_dscp_to_tc_color(sx_port_log_id_t port_id, mlnx_qos_map_t *qos_map)
{
    sx_status_t             status;
    uint32_t                count = SX_COS_PORT_DSCP_MAX + 1;
    sx_cos_dscp_t           dscp[SX_COS_PORT_DSCP_MAX + 1];
    sx_cos_priority_color_t prio_color[SX_COS_PORT_DSCP_MAX + 1];
    uint32_t                ii, jj;

    status = sx_api_cos_port_dscp_to_prio_get(gh_sdk, port_id, dscp, prio_color, &count);
    if (status != SX_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to get dscp to prio qos map - %s\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    for (ii = 0; ii < qos_map->count; ii++) {
        for (jj = 0; jj < count; jj++) {
            if (qos_map->from.dscp[ii] != dscp[jj]) {
                continue;
            }

            if (qos_map->type == SAI_QOS_MAP_DSCP_TO_TC) {
                prio_color[jj].priority = qos_map->to.prio_color[ii].priority;
            } else if (qos_map->type == SAI_QOS_MAP_DSCP_TO_COLOR) {
                prio_color[jj].color = qos_map->to.prio_color[ii].color;
            } else {
                /* We should not reach here but who knows ...*/
                assert(false);
            }
        }
    }

    status = sx_api_cos_port_dscp_to_prio_set(gh_sdk, port_id, dscp, prio_color, count);
    if (status != SX_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to set dscp to prio qos map - %s\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    return SAI_STATUS_SUCCESS;
}

static sai_status_t mlnx_port_qos_map_assign_tc_to_queue(sx_port_log_id_t port_id, mlnx_qos_map_t *qos_map)
{
    sx_status_t status;
    uint32_t    ii;

    for (ii = 0; ii < qos_map->count; ii++) {
        status = sx_api_cos_port_tc_prio_map_set(gh_sdk,
                                                 SX_ACCESS_CMD_ADD,
                                                 port_id,
                                                 qos_map->from.prio_color[ii].priority,
                                                 qos_map->to.queue[ii]);

        if (status != SX_STATUS_SUCCESS) {
            SX_LOG_ERR("Failed to set tc[%u]=%u -> queue[%u]=%u mapping - %s\n",
                       ii, qos_map->from.prio_color[ii].priority,
                       ii, qos_map->to.queue[ii], SX_STATUS_MSG(status));

            return sdk_to_sai(status);
        }
    }

    return SAI_STATUS_SUCCESS;
}

static sai_status_t mlnx_port_qos_map_assign_tc_color_to_dscp(sx_port_log_id_t port_id, mlnx_qos_map_t *qos_map)
{
    sx_status_t status;

    status = sx_api_cos_port_prio_to_dscp_rewrite_set(gh_sdk,
                                                      port_id,
                                                      qos_map->from.prio_color,
                                                      qos_map->to.dscp,
                                                      qos_map->count);
    if (status != SX_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to set prio/color to dscp qos map - %s\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    return SAI_STATUS_SUCCESS;
}

static sai_status_t mlnx_port_qos_map_assign_tc_color_to_dot1p(sx_port_log_id_t port_id, mlnx_qos_map_t *qos_map)
{
    sx_status_t status;

    status = sx_api_cos_port_prio_to_pcpdei_rewrite_set(gh_sdk, port_id,
                                                        qos_map->from.prio_color,
                                                        qos_map->to.pcp_dei,
                                                        qos_map->count);
    if (status != SX_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to set tc & color to dot1p qos map - %s\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    return SAI_STATUS_SUCCESS;
}

static sai_status_t mlnx_port_qos_map_assign_tc_to_pg(sx_port_log_id_t port_id, mlnx_qos_map_t *qos_map)
{
    sx_cos_port_prio_buff_t prio_buff;
    sx_status_t             status;
    uint32_t                ii;

    status = sx_api_cos_port_prio_buff_map_get(gh_sdk, port_id, &prio_buff);
    if (status != SX_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to get prio to buff qos map - %s\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    for (ii = 0; ii < qos_map->count; ii++) {
        uint8_t pri = qos_map->from.prio_color[ii].priority;

        prio_buff.prio_to_buff[pri] = qos_map->to.pg[ii];
    }

    status = sx_api_cos_port_prio_buff_map_set(gh_sdk, SX_ACCESS_CMD_SET, port_id, &prio_buff);
    if (status != SX_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to set prio to buff qos map - %s\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    return SAI_STATUS_SUCCESS;
}

static sai_status_t mlnx_port_qos_map_assign_pfc_to_pg(sx_port_log_id_t port_id, mlnx_qos_map_t *qos_map)
{
    sx_cos_port_prio_buff_t prio_buff;
    sx_status_t             status;
    uint32_t                ii, pri;

    status = sx_api_cos_port_prio_buff_map_get(gh_sdk, port_id, &prio_buff);
    if (status != SX_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to get prio to buff qos map - %s\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    for (ii = 0; ii < qos_map->count; ii++) {
        uint8_t            pfc = qos_map->from.pfc[ii];
        uint8_t            pg  = qos_map->to.pg[ii];
        sx_cos_priority_t  prios[SXD_COS_PORT_PRIO_MAX];
        sx_cos_ieee_prio_t ieees[SXD_COS_PORT_PRIO_MAX];
        uint32_t           count = 0;

        for (pri = 0; pri < SXD_COS_PORT_PRIO_MAX + 1; pri++) {
            if (pg != prio_buff.prio_to_buff[pri]) {
                continue;
            }

            ieees[count] = pfc;
            prios[count] = pri;
            count++;
        }

        if (!count) {
            continue;
        }

        status = sx_api_cos_prio_to_ieeeprio_set(gh_sdk, prios, ieees, count);
        if (status != SX_STATUS_SUCCESS) {
            SX_LOG_ERR("Failed to set prio to ieee qos map - %s\n", SX_STATUS_MSG(status));
            return sdk_to_sai(status);
        }
    }

    return status;
}

static sai_status_t mlnx_port_qos_map_assign_pfc_to_queue(sx_port_log_id_t port_id, mlnx_qos_map_t *qos_map)
{
    sx_cos_priority_t  prios[SXD_COS_PORT_PRIO_MAX];
    sx_cos_ieee_prio_t ieees[SXD_COS_PORT_PRIO_MAX];
    uint32_t           count = SXD_COS_PORT_PRIO_MAX;
    sx_status_t        status;
    uint32_t           ii, jj;

    status = sx_api_cos_prio_to_ieeeprio_get(gh_sdk, prios, ieees, &count);
    if (status != SX_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to get prio to ieee qos map - %s\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    for (ii = 0; ii < qos_map->count; ii++) {
        for (jj = 0; jj < count; jj++) {
            if (ieees[jj] != qos_map->from.pfc[ii]) {
                continue;
            }

            status = sx_api_cos_port_tc_prio_map_set(gh_sdk,
                                                     SX_ACCESS_CMD_ADD,
                                                     port_id,
                                                     prios[jj],
                                                     qos_map->to.queue[ii]);

            if (status != SX_STATUS_SUCCESS) {
                SX_LOG_ERR("Failed to set tc[%u]=%u -> queue[%u]=%u mapping - %s\n",
                           ii, prios[jj],
                           ii, qos_map->to.queue[ii], SX_STATUS_MSG(status));

                return sdk_to_sai(status);
            }
        }
    }

    return status;
}

/*
 * Routine Description:
 *   Apply QoS params on the port (db read lock is needed).
 *
 * Arguments:
 *    [in] port - Port Id
 *    [in] qos_map_id - QoS Map Id
 *    [in] qos_map_type QoS Map Type
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t mlnx_port_qos_map_apply(_In_ const sai_object_id_t    port,
                                     _In_ const sai_object_id_t    qos_map_id,
                                     _In_ const sai_qos_map_type_t qos_map_type)
{
    bool             is_map_enabled = true;
    mlnx_qos_map_t   default_map;
    mlnx_qos_map_t  *qos_map = NULL;
    sai_status_t     status;
    sx_port_log_id_t port_id;

    status = mlnx_object_to_log_port(port, &port_id);
    if (status != SAI_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to convert port oid to logical port id\n");
        return status;
    }

    if (qos_map_id != SAI_NULL_OBJECT_ID) {
        status = mlnx_qos_map_get_by_id(qos_map_id, &qos_map);
        if (status != SAI_STATUS_SUCCESS) {
            SX_LOG_ERR("Invalid qos_map_id\n");
            return status;
        }
    } else if (g_sai_db_ptr->switch_qos_maps[qos_map_type]) {
        uint32_t        map_idx = g_sai_db_ptr->switch_qos_maps[qos_map_type];
        sai_object_id_t map_oid;

        status = mlnx_create_object(SAI_OBJECT_TYPE_QOS_MAPS, map_idx, NULL, &map_oid);
        if (status != SAI_STATUS_SUCCESS) {
            SX_LOG_ERR("Failed create oid by switch qos map_idx=%u\n", map_idx);
            return status;
        }

        status = mlnx_qos_map_get_by_id(map_oid, &qos_map);
        if (status != SAI_STATUS_SUCCESS) {
            SX_LOG_ERR("Invalid qos_map_id from switch qos map oid=%" PRIx64 "\n", map_oid);
            return status;
        }
    } else {
        is_map_enabled = false;
        qos_map        = &default_map;
        qos_map->type  = qos_map_type;

        status = mlnx_qos_map_set_default(qos_map);
        if (status != SAI_STATUS_SUCCESS) {
            SX_LOG_ERR("Failed to set default QoS map\n");
            return status;
        }
    }

    if (qos_map->type != qos_map_type) {
        SX_LOG_ERR("Specified QoS map's type does not match port's QoS map attr type\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    switch (qos_map_type) {
    case SAI_QOS_MAP_DOT1P_TO_TC:
    case SAI_QOS_MAP_DOT1P_TO_COLOR:
        status = mlnx_port_qos_map_assign_dot1p_to_tc_color(port_id, qos_map);
        if (status != SAI_STATUS_SUCCESS) {
            return status;
        }

        status = mlnx_port_qos_map_trust_level_set(port_id, qos_map, is_map_enabled);
        break;

    case SAI_QOS_MAP_DSCP_TO_TC:
    case SAI_QOS_MAP_DSCP_TO_COLOR:
        status = mlnx_port_qos_map_assign_dscp_to_tc_color(port_id, qos_map);
        if (status != SAI_STATUS_SUCCESS) {
            return status;
        }

        status = mlnx_port_qos_map_trust_level_set(port_id, qos_map, is_map_enabled);
        break;

    case SAI_QOS_MAP_TC_TO_QUEUE:
        status = mlnx_port_qos_map_assign_tc_to_queue(port_id, qos_map);
        break;

    case SAI_QOS_MAP_TC_AND_COLOR_TO_DSCP:
        status = mlnx_port_qos_map_assign_tc_color_to_dscp(port_id, qos_map);
        break;

    case SAI_QOS_MAP_TC_AND_COLOR_TO_DOT1P:
        status = mlnx_port_qos_map_assign_tc_color_to_dot1p(port_id, qos_map);
        break;

    case SAI_QOS_MAP_TC_TO_PRIORITY_GROUP:
        if (is_map_enabled) {
            status = mlnx_port_qos_map_assign_tc_to_pg(port_id, qos_map);
        }
        break;

    case SAI_QOS_MAP_PFC_PRIORITY_TO_PRIORITY_GROUP:
        if (is_map_enabled) {
            status = mlnx_port_qos_map_assign_pfc_to_pg(port_id, qos_map);
        }
        break;

    case SAI_QOS_MAP_PFC_PRIORITY_TO_QUEUE:
        if (is_map_enabled) {
            status = mlnx_port_qos_map_assign_pfc_to_queue(port_id, qos_map);
        }
        break;

    case SAI_QOS_MAP_CUSTOM_RANGE_BASE:
    default:
        status = SAI_STATUS_NOT_SUPPORTED;
        SX_LOG_ERR("Not supported qos_map_type (%u)\n", qos_map_type);
        return status;
    }

    return status;
}

/*
 * Routine Description:
 *   Set default traffic class on the port
 *
 * Arguments:
 *    [in] port - Port Id
 *    [in] tc - traffic class
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t mlnx_port_tc_set(mlnx_port_config_t *port, _In_ const uint8_t tc)
{
    sai_status_t status;

    status = sx_api_cos_port_default_prio_set(gh_sdk, port->logical, tc);
    if (status != SX_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to set port's default tc(%u) - %s\n", tc, SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *   Get default traffic class on the port
 *
 * Arguments:
 *    [in] port - Port Id
 *    [out] tc - traffic class
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
static sai_status_t mlnx_port_tc_get(_In_ const sai_object_id_t port, _Out_ uint8_t *tc)
{
    sx_port_log_id_t port_id;
    sai_status_t     status;

    status = mlnx_object_to_type(port, SAI_OBJECT_TYPE_PORT, &port_id, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        return status;
    }

    status = sx_api_cos_port_default_prio_get(gh_sdk, port_id, tc);
    if (status != SX_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to get port's default traffic class - %s\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    return SAI_STATUS_SUCCESS;
}

/** QoS Map Id [sai_object_id_t] */
static sai_status_t mlnx_port_qos_map_id_set(_In_ const sai_object_key_t      *key,
                                             _In_ const sai_attribute_value_t *value,
                                             void                             *arg)
{
    sai_qos_map_type_t qos_map_type = (sai_qos_map_type_t)arg;
    sx_port_log_id_t   port_id;
    sai_status_t       status;

    SX_LOG_ENTER();

    assert(qos_map_type < MLNX_QOS_MAP_TYPES_MAX);

    status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL);
    if (SAI_ERR(status)) {
        return status;
    }

    sai_db_write_lock();

    status = mlnx_port_qos_map_apply(key->object_id, value->oid, qos_map_type);
    if (status != SAI_STATUS_SUCCESS) {
        goto out;
    }

    status = db_port_qos_map_id_set(key->object_id, qos_map_type, value->oid);
    if (status != SAI_STATUS_SUCCESS) {
        goto out;
    }

    sai_db_sync();

out:
    sai_db_unlock();

    SX_LOG_EXIT();
    return status;
}

/** bit vector enable/disable port PFC [sai_uint8_t].
 * Valid from bit 0 to bit 7 */
static sai_status_t mlnx_port_pfc_control_get(_In_ const sai_object_key_t   *key,
                                              _Inout_ sai_attribute_value_t *value,
                                              _In_ uint32_t                  attr_index,
                                              _Inout_ vendor_cache_t        *cache,
                                              void                          *arg)
{
    sx_port_log_id_t port_id;
    uint8_t          pfc_ctrl_map = 0;
    sai_status_t     status;
    uint8_t          pfc_prio;

    SX_LOG_ENTER();

    status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to convert port oid to logical port id\n");
        return status;
    }

    for (pfc_prio = 0; pfc_prio < COS_IEEE_PRIO_MAX_NUM + 1; pfc_prio++) {
        sx_port_flow_ctrl_mode_t flow_mode = SX_PORT_FLOW_CTRL_MODE_TX_DIS_RX_DIS;

        status = sx_api_port_pfc_enable_get(gh_sdk, port_id, pfc_prio, &flow_mode);
        if (status != SAI_STATUS_SUCCESS) {
            SX_LOG_ERR("Failed to get pfc control for prio=%u\n", pfc_prio);
            return sdk_to_sai(status);
        }

        if (flow_mode == SX_PORT_FLOW_CTRL_MODE_TX_EN_RX_EN) {
            pfc_ctrl_map |= (1 << pfc_prio);
        }
    }

    value->u8 = pfc_ctrl_map;

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/** bit vector enable/disable port PFC [sai_uint8_t].
 * Valid from bit 0 to bit 7 */
static sai_status_t mlnx_port_pfc_control_set(_In_ const sai_object_key_t      *key,
                                              _In_ const sai_attribute_value_t *value,
                                              void                             *arg)
{
    sx_port_log_id_t port_id;
    sai_status_t     status;
    uint8_t          pfc_prio;

    SX_LOG_ENTER();

    status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        SX_LOG_ERR("Failed to convert port oid to logical port id\n");
        return status;
    }

    for (pfc_prio = 0; pfc_prio < COS_IEEE_PRIO_MAX_NUM + 1; pfc_prio++) {
        sx_port_flow_ctrl_mode_t flow_mode = SX_PORT_FLOW_CTRL_MODE_TX_DIS_RX_DIS;

        if (value->u8 & (1 << pfc_prio)) {
            flow_mode = SX_PORT_FLOW_CTRL_MODE_TX_EN_RX_EN;
        }

        status = sx_api_port_pfc_enable_set(gh_sdk, port_id, pfc_prio, flow_mode);
        if (status != SAI_STATUS_SUCCESS) {
            SX_LOG_ERR("Failed to enable/disable pfc control for prio=%u\n", pfc_prio);
            return sdk_to_sai(status);
        }
    }

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Get WRED profile set to a port */
static sai_status_t mlnx_port_wred_get(_In_ const sai_object_key_t   *key,
                                       _Inout_ sai_attribute_value_t *value,
                                       _In_ uint32_t                  attr_index,
                                       _Inout_ vendor_cache_t        *cache,
                                       void                          *arg)
{
    sai_status_t     status      = SAI_STATUS_SUCCESS;
    sai_object_id_t  wred_id_val = SAI_NULL_OBJECT_ID;
    sx_port_log_id_t port_id;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL))) {
        return status;
    }

    if (SAI_STATUS_SUCCESS != (status = mlnx_wred_get_wred_id(key->object_id, &wred_id_val))) {
        SX_LOG_ERR("Failed to get WRED for the port 0x%x\n", port_id);
        return status;
    }

    value->oid = wred_id_val;

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Get number of queues for the port */
static sai_status_t mlnx_port_queue_num_get(_In_ const sai_object_key_t   *key,
                                            _Inout_ sai_attribute_value_t *value,
                                            _In_ uint32_t                  attr_index,
                                            _Inout_ vendor_cache_t        *cache,
                                            void                          *arg)
{
    sai_status_t     status = SAI_STATUS_SUCCESS;
    sx_port_log_id_t port_id;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL))) {
        return status;
    }

    value->u32 = g_resource_limits.cos_port_ets_traffic_class_max + 1;

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Get list of queues for the port */
static sai_status_t mlnx_port_queue_list_get(_In_ const sai_object_key_t   *key,
                                             _Inout_ sai_attribute_value_t *value,
                                             _In_ uint32_t                  attr_index,
                                             _Inout_ vendor_cache_t        *cache,
                                             void                          *arg)
{
    sai_status_t     status = SAI_STATUS_SUCCESS;
    sx_port_log_id_t port_id;
    uint32_t         ii = 0;
    sai_object_id_t *port_queues;

    SX_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL))) {
        return status;
    }

    port_queues = malloc(sizeof(sai_object_id_t) * (g_resource_limits.cos_port_ets_traffic_class_max + 1));

    for (ii = 0; ii <= g_resource_limits.cos_port_ets_traffic_class_max; ii++) {
        if (SAI_STATUS_SUCCESS != (status = mlnx_create_queue_object(port_id, ii, &port_queues[ii]))) {
            SX_LOG_ERR("Failed to create SAI object for port 0x%x TC = %u", port_id, ii);
            goto out;
        }
    }

    status = mlnx_fill_objlist(port_queues, ii, &value->objlist);

out:
    SX_LOG_EXIT();
    free(port_queues);
    return status;
}

static uint32_t sched_groups_count(mlnx_port_config_t *port)
{
    uint32_t count = 0;
    uint32_t ii;

    for (ii = 0; ii < MAX_SCHED_LEVELS; ii++) {
        count += port->sched_hierarchy.groups_count[ii];
    }

    return count;
}

/** Number of Scheduler groups on port [uint32_t]*/
static sai_status_t mlnx_port_sched_groups_num_get(_In_ const sai_object_key_t   *key,
                                                   _Inout_ sai_attribute_value_t *value,
                                                   _In_ uint32_t                  attr_index,
                                                   _Inout_ vendor_cache_t        *cache,
                                                   void                          *arg)
{
    sai_status_t        status;
    sx_port_log_id_t    port_id;
    mlnx_port_config_t *port;

    SX_LOG_ENTER();

    status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        return status;
    }

    sai_qos_db_read_lock();

    status = mlnx_port_by_log_id(port_id, &port);
    if (SAI_ERR(status)) {
        goto out;
    }

    value->u32 = sched_groups_count(port);

out:
    sai_qos_db_unlock();
    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/** List of Scheduler groups for the port[sai_object_list_t] */
static sai_status_t mlnx_port_sched_groups_list_get(_In_ const sai_object_key_t   *key,
                                                    _Inout_ sai_attribute_value_t *value,
                                                    _In_ uint32_t                  attr_index,
                                                    _Inout_ vendor_cache_t        *cache,
                                                    void                          *arg)
{
    sai_status_t        status = SAI_STATUS_SUCCESS;
    mlnx_port_config_t *port;
    sx_port_log_id_t    port_id;
    uint32_t            lvl, idx, count;
    sai_object_id_t    *groups = NULL;

    SX_LOG_ENTER();

    status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        return status;
    }

    sai_qos_db_read_lock();

    status = mlnx_port_by_log_id(port_id, &port);
    if (SAI_ERR(status)) {
        goto out;
    }

    count = sched_groups_count(port);

    groups = malloc(count * sizeof(sai_object_id_t));
    if (!groups) {
        SX_LOG_ERR("Failed to allocate scheduler groups list\n");
        status = SAI_STATUS_NO_MEMORY;
        goto out;
    }

    for (lvl = 0, idx = 0; lvl < MAX_SCHED_LEVELS; lvl++) {
        uint8_t count = (lvl == 0) ? 1 : MAX_SCHED_CHILD_GROUPS;
        uint8_t ii;

        for (ii = 0; ii < count; ii++, idx++) {
            sai_status_t status;

            if (!port->sched_hierarchy.groups[lvl][ii].is_used) {
                continue;
            }

            status = mlnx_create_sched_group(port_id, lvl, ii, &groups[idx]);
            if (SAI_ERR(status)) {
                goto out;
            }
        }
    }

    status = mlnx_fill_objlist(groups, count, &value->objlist);

out:
    sai_qos_db_unlock();
    SX_LOG_EXIT();
    free(groups);
    return status;
}

/** Scheduler for port [sai_object_id_t], Default no limits.
 * SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE & SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE
 * attributes alone valid. Rest will be ignored */
static sai_status_t mlnx_port_sched_get(_In_ const sai_object_key_t   *key,
                                        _Inout_ sai_attribute_value_t *value,
                                        _In_ uint32_t                  attr_index,
                                        _Inout_ vendor_cache_t        *cache,
                                        void                          *arg)
{
    sx_port_log_id_t    port_log_id;
    sai_status_t        status;
    mlnx_port_config_t *port;

    status = mlnx_object_to_log_port(key->object_id, &port_log_id);
    if (status != SAI_STATUS_SUCCESS) {
        return status;
    }

    sai_qos_db_read_lock();

    status = mlnx_port_by_log_id(port_log_id, &port);
    if (status != SAI_STATUS_SUCCESS) {
        sai_qos_db_unlock();
        return status;
    }
    value->oid = port->scheduler_id;

    sai_qos_db_unlock();

    return status;
}

/** Scheduler for port [sai_object_id_t], Default no limits.
 * SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE & SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE
 * attributes alone valid. Rest will be ignored */
static sai_status_t mlnx_port_sched_set(_In_ const sai_object_key_t      *key,
                                        _In_ const sai_attribute_value_t *value,
                                        void                             *arg)
{
    return mlnx_scheduler_to_port_apply(value->oid, key->object_id);
}

static void port_key_to_str(_In_ sai_object_id_t port_id, _Out_ char *key_str)
{
    char        *type_str = "port";
    sai_status_t status;
    uint32_t     port;

    status = mlnx_object_to_log_port(port_id, &port);
    if (SAI_ERR(status)) {
        snprintf(key_str, MAX_KEY_STR_LEN, "invalid port/lag");
        return;
    }

    if (sai_object_type_query(port_id) == SAI_OBJECT_TYPE_LAG) {
        type_str = "lag";
    }

    snprintf(key_str, MAX_KEY_STR_LEN, "%s %x", type_str, port);
}

/*
 * Routine Description:
 *   Set port attribute value.
 *
 * Arguments:
 *    [in] port_id - port id
 *    [in] attr - attribute
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
static sai_status_t mlnx_set_port_attribute(_In_ sai_object_id_t port_id, _In_ const sai_attribute_t *attr)
{
    const sai_object_key_t key = { .object_id = port_id };
    char                   key_str[MAX_KEY_STR_LEN];
    sai_status_t           sai_status;

    SX_LOG_ENTER();

    sai_status = check_attrs_port_type(&key, 1, attr);
    if (SAI_ERR(sai_status)) {
        return sai_status;
    }

    port_key_to_str(port_id, key_str);
    sai_status = sai_set_attribute(&key, key_str, port_attribs, port_vendor_attribs, attr);
    SX_LOG_EXIT();
    return sai_status;
}


/*
 * Routine Description:
 *   Get port attribute value.
 *
 * Arguments:
 *    [in] port_id - port id
 *    [in] attr_count - number of attributes
 *    [inout] attr_list - array of attributes
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
static sai_status_t mlnx_get_port_attribute(_In_ sai_object_id_t     port_id,
                                            _In_ uint32_t            attr_count,
                                            _Inout_ sai_attribute_t *attr_list)
{
    const sai_object_key_t key = { .object_id = port_id };
    char                   key_str[MAX_KEY_STR_LEN];
    sai_status_t           sai_status;

    SX_LOG_ENTER();

    port_key_to_str(port_id, key_str);
    sai_status = sai_get_attributes(&key, key_str, port_attribs, port_vendor_attribs, attr_count, attr_list);
    SX_LOG_EXIT();
    return sai_status;
}

/*
 * Routine Description:
 *   Get port statistics counters.
 *
 * Arguments:
 *    [in] port_id - port id
 *    [in] counter_ids - specifies the array of counter ids
 *    [in] number_of_counters - number of counters in the array
 *    [out] counters - array of resulting counter values.
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
static sai_status_t mlnx_get_port_stats(_In_ sai_object_id_t                port_id,
                                        _In_ const sai_port_stat_counter_t *counter_ids,
                                        _In_ uint32_t                       number_of_counters,
                                        _Out_ uint64_t                     *counters)
{
    sai_status_t                  status;
    sx_port_cntr_rfc_2863_t       cnts_2863;
    sx_port_cntr_rfc_2819_t       cnts_2819;
    sx_port_cntr_prio_t           cntr_prio;
    sx_port_cntr_ieee_802_dot_3_t cntr_802;
    sx_cos_redecn_port_counters_t redecn_cnts;
    uint32_t                      ii, port_data;
    uint32_t                      iter = 0;
    char                          key_str[MAX_KEY_STR_LEN];

    SX_LOG_ENTER();

    memset(&redecn_cnts, 0, sizeof(redecn_cnts));

    port_key_to_str(port_id, key_str);
    SX_LOG_DBG("Get port stats %s\n", key_str);

    if (NULL == counter_ids) {
        SX_LOG_ERR("NULL counter ids array param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (NULL == counters) {
        SX_LOG_ERR("NULL counters array param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_type(port_id, SAI_OBJECT_TYPE_PORT, &port_data, NULL))) {
        return status;
    }

    if (SX_STATUS_SUCCESS !=
        (status = sx_api_port_counter_rfc_2863_get(gh_sdk, SX_ACCESS_CMD_READ, port_data, &cnts_2863))) {
        SX_LOG_ERR("Failed to get port rfc 2863 counters - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    if (SX_STATUS_SUCCESS !=
        (status = sx_api_port_counter_rfc_2819_get(gh_sdk, SX_ACCESS_CMD_READ, port_data, &cnts_2819))) {
        SX_LOG_ERR("Failed to get port rfc 2819 counters - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    if (SX_STATUS_SUCCESS !=
        (status = sx_api_port_counter_ieee_802_dot_3_get(gh_sdk, SX_ACCESS_CMD_READ, port_data, &cntr_802))) {
        SX_LOG_ERR("Failed to get port ieee 802 3 counters - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    for (ii = 0; ii < number_of_counters; ii++) {
        switch (counter_ids[ii]) {
        case SAI_PORT_STAT_IF_IN_OCTETS:
            counters[ii] = cnts_2863.if_in_octets;
            break;

        case SAI_PORT_STAT_IF_IN_UCAST_PKTS:
            counters[ii] = cnts_2863.if_in_ucast_pkts;
            break;

        case SAI_PORT_STAT_IF_IN_NON_UCAST_PKTS:
            counters[ii] = cnts_2863.if_in_broadcast_pkts + cnts_2863.if_in_multicast_pkts;
            break;

        case SAI_PORT_STAT_IF_IN_DISCARDS:
            counters[ii] = cnts_2863.if_in_discards;
            break;

        case SAI_PORT_STAT_IF_IN_ERRORS:
            counters[ii] = cnts_2863.if_in_errors;
            break;

        case SAI_PORT_STAT_IF_IN_UNKNOWN_PROTOS:
            counters[ii] = cnts_2863.if_in_unknown_protos;
            break;

        case SAI_PORT_STAT_IF_IN_BROADCAST_PKTS:
            counters[ii] = cnts_2863.if_in_broadcast_pkts;
            break;

        case SAI_PORT_STAT_IF_IN_MULTICAST_PKTS:
            counters[ii] = cnts_2863.if_in_multicast_pkts;
            break;

        case SAI_PORT_STAT_IF_OUT_OCTETS:
            counters[ii] = cnts_2863.if_out_octets;
            break;

        case SAI_PORT_STAT_IF_OUT_UCAST_PKTS:
            counters[ii] = cnts_2863.if_out_ucast_pkts;
            break;

        case SAI_PORT_STAT_IF_OUT_NON_UCAST_PKTS:
            counters[ii] = cnts_2863.if_out_broadcast_pkts + cnts_2863.if_out_multicast_pkts;
            break;

        case SAI_PORT_STAT_IF_OUT_DISCARDS:
            counters[ii] = cnts_2863.if_out_discards;
            break;

        case SAI_PORT_STAT_IF_OUT_ERRORS:
            counters[ii] = cnts_2863.if_out_errors;
            break;

        case SAI_PORT_STAT_IF_OUT_BROADCAST_PKTS:
            counters[ii] = cnts_2863.if_out_broadcast_pkts;
            break;

        case SAI_PORT_STAT_IF_OUT_MULTICAST_PKTS:
            counters[ii] = cnts_2863.if_out_multicast_pkts;
            break;

        case SAI_PORT_STAT_ETHER_STATS_DROP_EVENTS:
            counters[ii] = cnts_2819.ether_stats_drop_events;
            break;

        case SAI_PORT_STAT_ETHER_STATS_MULTICAST_PKTS:
            counters[ii] = cnts_2819.ether_stats_multicast_pkts;
            break;

        case SAI_PORT_STAT_ETHER_STATS_BROADCAST_PKTS:
            counters[ii] = cnts_2819.ether_stats_broadcast_pkts;
            break;

        case SAI_PORT_STAT_ETHER_STATS_UNDERSIZE_PKTS:
            counters[ii] = cnts_2819.ether_stats_undersize_pkts;
            break;

        case SAI_PORT_STAT_ETHER_STATS_FRAGMENTS:
            counters[ii] = cnts_2819.ether_stats_fragments;
            break;

        case SAI_PORT_STAT_ETHER_STATS_PKTS_64_OCTETS:
            counters[ii] = cnts_2819.ether_stats_pkts64octets;
            break;

        case SAI_PORT_STAT_ETHER_STATS_PKTS_65_TO_127_OCTETS:
            counters[ii] = cnts_2819.ether_stats_pkts65to127octets;
            break;

        case SAI_PORT_STAT_ETHER_STATS_PKTS_128_TO_255_OCTETS:
            counters[ii] = cnts_2819.ether_stats_pkts128to255octets;
            break;

        case SAI_PORT_STAT_ETHER_STATS_PKTS_256_TO_511_OCTETS:
            counters[ii] = cnts_2819.ether_stats_pkts256to511octets;
            break;

        case SAI_PORT_STAT_ETHER_STATS_PKTS_512_TO_1023_OCTETS:
            counters[ii] = cnts_2819.ether_stats_pkts512to1023octets;
            break;

        case SAI_PORT_STAT_ETHER_STATS_PKTS_1024_TO_1518_OCTETS:
            counters[ii] = cnts_2819.ether_stats_pkts1024to1518octets;
            break;

        case SAI_PORT_STAT_ETHER_STATS_OVERSIZE_PKTS:
            counters[ii] = cnts_2819.ether_stats_oversize_pkts;
            break;

        case SAI_PORT_STAT_ETHER_STATS_JABBERS:
            counters[ii] = cnts_2819.ether_stats_jabbers;
            break;

        case SAI_PORT_STAT_ETHER_STATS_OCTETS:
            counters[ii] = cnts_2819.ether_stats_octets;
            break;

        case SAI_PORT_STAT_ETHER_STATS_PKTS:
            counters[ii] = cnts_2819.ether_stats_pkts;
            break;

        case SAI_PORT_STAT_ETHER_STATS_COLLISIONS:
            counters[ii] = cnts_2819.ether_stats_collisions;
            break;

        case SAI_PORT_STAT_ETHER_STATS_CRC_ALIGN_ERRORS:
            counters[ii] = cnts_2819.ether_stats_crc_align_errors;
            break;

        case SAI_PORT_STAT_PAUSE_RX_PKTS:
            counters[ii] = cntr_802.a_pause_mac_ctrl_frames_received;
            break;

        case SAI_PORT_STAT_PAUSE_TX_PKTS:
            counters[ii] = cntr_802.a_pause_mac_ctrl_frames_transmitted;
            break;

        case SAI_PORT_STAT_GREEN_DISCARD_DROPPED_PACKETS:
        case SAI_PORT_STAT_GREEN_DISCARD_DROPPED_BYTES:
        case SAI_PORT_STAT_YELLOW_DISCARD_DROPPED_PACKETS:
        case SAI_PORT_STAT_YELLOW_DISCARD_DROPPED_BYTES:
        case SAI_PORT_STAT_RED_DISCARD_DROPPED_PACKETS:
        case SAI_PORT_STAT_RED_DISCARD_DROPPED_BYTES:
        case SAI_PORT_STAT_DISCARD_DROPPED_BYTES:
            SX_LOG_ERR("Port counter %d set item %u not supported\n", counter_ids[ii], ii);
            return SAI_STATUS_ATTR_NOT_SUPPORTED_0;

        case SAI_PORT_STAT_DISCARD_DROPPED_PACKETS:
            if (SX_STATUS_SUCCESS !=
                (status = sx_api_cos_redecn_counters_get(gh_sdk, SX_ACCESS_CMD_READ, port_data, &redecn_cnts))) {
                SX_LOG_ERR("Failed to get port redecn counters - %s.\n", SX_STATUS_MSG(status));
                return sdk_to_sai(status);
            }
            counters[ii] = 0;
            /* TODO : change to  g_resource_limits.cos_port_ets_traffic_class_max + 1 when sdk is updated to use rm */
            for (iter = 0; iter < RM_API_COS_TRAFFIC_CLASS_NUM; iter++) {
                counters[ii] += redecn_cnts.tc_red_dropped_packets[iter];
            }
            break;

        case SAI_PORT_STAT_ECN_MARKED_PACKETS:
            if (SX_STATUS_SUCCESS !=
                (status = sx_api_cos_redecn_counters_get(gh_sdk, SX_ACCESS_CMD_READ, port_data, &redecn_cnts))) {
                SX_LOG_ERR("Failed to get port redecn counters - %s.\n", SX_STATUS_MSG(status));
                return sdk_to_sai(status);
            }
            counters[ii] = redecn_cnts.ecn_marked_packets;
            break;

        case SAI_PORT_STAT_PFC_0_RX_PKTS:
        case SAI_PORT_STAT_PFC_1_RX_PKTS:
        case SAI_PORT_STAT_PFC_2_RX_PKTS:
        case SAI_PORT_STAT_PFC_3_RX_PKTS:
        case SAI_PORT_STAT_PFC_4_RX_PKTS:
        case SAI_PORT_STAT_PFC_5_RX_PKTS:
        case SAI_PORT_STAT_PFC_6_RX_PKTS:
        case SAI_PORT_STAT_PFC_7_RX_PKTS:
            if (SX_STATUS_SUCCESS !=
                (status = sx_api_port_counter_prio_get(gh_sdk, SX_ACCESS_CMD_READ, port_data,
                                                       /* Extract Prio i from SAI RXi,TXi */
                                                       SX_PORT_PRIO_ID_0 +
                                                       (counter_ids[ii] - SAI_PORT_STAT_PFC_0_RX_PKTS) / 2,
                                                       &cntr_prio))) {
                SX_LOG_ERR("Failed to get port prio %d counters - %s.\n",
                           SX_PORT_PRIO_ID_0 + (counter_ids[ii] - SAI_PORT_STAT_PFC_0_RX_PKTS) / 2,
                           SX_STATUS_MSG(status));
                return sdk_to_sai(status);
            }
            counters[ii] = cntr_prio.rx_pause;
            break;

        case SAI_PORT_STAT_PFC_0_TX_PKTS:
        case SAI_PORT_STAT_PFC_1_TX_PKTS:
        case SAI_PORT_STAT_PFC_2_TX_PKTS:
        case SAI_PORT_STAT_PFC_3_TX_PKTS:
        case SAI_PORT_STAT_PFC_4_TX_PKTS:
        case SAI_PORT_STAT_PFC_5_TX_PKTS:
        case SAI_PORT_STAT_PFC_6_TX_PKTS:
        case SAI_PORT_STAT_PFC_7_TX_PKTS:
            if (SX_STATUS_SUCCESS !=
                (status = sx_api_port_counter_prio_get(gh_sdk, SX_ACCESS_CMD_READ, port_data,
                                                       SX_PORT_PRIO_ID_0 +
                                                       (counter_ids[ii] - SAI_PORT_STAT_PFC_0_TX_PKTS) / 2,
                                                       &cntr_prio))) {
                SX_LOG_ERR("Failed to get port prio %d counters - %s.\n",
                           SX_PORT_PRIO_ID_0 + (counter_ids[ii] - SAI_PORT_STAT_PFC_0_TX_PKTS) / 2,
                           SX_STATUS_MSG(status));
                return sdk_to_sai(status);
            }
            counters[ii] = cntr_prio.tx_pause;
            break;

        case SAI_PORT_STAT_IF_IN_VLAN_DISCARDS:
        case SAI_PORT_STAT_IF_OUT_QLEN:
        case SAI_PORT_STAT_ETHER_RX_OVERSIZE_PKTS:
        case SAI_PORT_STAT_ETHER_TX_OVERSIZE_PKTS:
        case SAI_PORT_STAT_ETHER_STATS_TX_NO_ERRORS:
        case SAI_PORT_STAT_ETHER_STATS_RX_NO_ERRORS:
        case SAI_PORT_STAT_IP_IN_RECEIVES:
        case SAI_PORT_STAT_IP_IN_OCTETS:
        case SAI_PORT_STAT_IP_IN_UCAST_PKTS:
        case SAI_PORT_STAT_IP_IN_NON_UCAST_PKTS:
        case SAI_PORT_STAT_IP_IN_DISCARDS:
        case SAI_PORT_STAT_IP_OUT_OCTETS:
        case SAI_PORT_STAT_IP_OUT_UCAST_PKTS:
        case SAI_PORT_STAT_IP_OUT_NON_UCAST_PKTS:
        case SAI_PORT_STAT_IP_OUT_DISCARDS:
        case SAI_PORT_STAT_IPV6_IN_RECEIVES:
        case SAI_PORT_STAT_IPV6_IN_OCTETS:
        case SAI_PORT_STAT_IPV6_IN_UCAST_PKTS:
        case SAI_PORT_STAT_IPV6_IN_NON_UCAST_PKTS:
        case SAI_PORT_STAT_IPV6_IN_MCAST_PKTS:
        case SAI_PORT_STAT_IPV6_IN_DISCARDS:
        case SAI_PORT_STAT_IPV6_OUT_OCTETS:
        case SAI_PORT_STAT_IPV6_OUT_UCAST_PKTS:
        case SAI_PORT_STAT_IPV6_OUT_NON_UCAST_PKTS:
        case SAI_PORT_STAT_IPV6_OUT_MCAST_PKTS:
        case SAI_PORT_STAT_IPV6_OUT_DISCARDS:
        case SAI_PORT_STAT_ETHER_IN_PKTS_64_OCTETS:
        case SAI_PORT_STAT_ETHER_IN_PKTS_65_TO_127_OCTETS:
        case SAI_PORT_STAT_ETHER_IN_PKTS_128_TO_255_OCTETS:
        case SAI_PORT_STAT_ETHER_IN_PKTS_256_TO_511_OCTETS:
        case SAI_PORT_STAT_ETHER_IN_PKTS_512_TO_1023_OCTETS:
        case SAI_PORT_STAT_ETHER_IN_PKTS_1024_TO_1518_OCTETS:
        case SAI_PORT_STAT_ETHER_OUT_PKTS_64_OCTETS:
        case SAI_PORT_STAT_ETHER_OUT_PKTS_65_TO_127_OCTETS:
        case SAI_PORT_STAT_ETHER_OUT_PKTS_128_TO_255_OCTETS:
        case SAI_PORT_STAT_ETHER_OUT_PKTS_256_TO_511_OCTETS:
        case SAI_PORT_STAT_ETHER_OUT_PKTS_512_TO_1023_OCTETS:
        case SAI_PORT_STAT_ETHER_OUT_PKTS_1024_TO_1518_OCTETS:
        case SAI_PORT_STAT_CURR_OCCUPANCY_BYTES:
        case SAI_PORT_STAT_WATERMARK_BYTES:
        case SAI_PORT_STAT_SHARED_CURR_OCCUPANCY_BYTES:
        case SAI_PORT_STAT_SHARED_WATERMARK_BYTES:
            SX_LOG_ERR("Port counter %d set item %u not implemented\n", counter_ids[ii], ii);
            return SAI_STATUS_NOT_IMPLEMENTED;

        default:
            SX_LOG_ERR("Invalid port counter %d\n", counter_ids[ii]);
            return SAI_STATUS_INVALID_PARAMETER;
        }
    }

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *   Clear port statistics counters.
 *
 * Arguments:
 *    [in] port_id - port id
 *    [in] counter_ids - specifies the array of counter ids
 *    [in] number_of_counters - number of counters in the array
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
static sai_status_t mlnx_clear_port_stats(_In_ sai_object_id_t                port_id,
                                          _In_ const sai_port_stat_counter_t *counter_ids,
                                          _In_ uint32_t                       number_of_counters)
{
    return SAI_STATUS_NOT_IMPLEMENTED;
}

/*
 * Routine Description:
 *   Clear port's all statistics counters.
 *
 * Arguments:
 *    [in] port_id - port id
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
static sai_status_t mlnx_clear_port_all_stats(_In_ sai_object_id_t port_id)
{
    sai_status_t status;
    uint32_t     port_data;
    char         key_str[MAX_KEY_STR_LEN];

    SX_LOG_ENTER();

    port_key_to_str(port_id, key_str);
    SX_LOG_NTC("Clear all port stats %s\n", key_str);

    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_type(port_id, SAI_OBJECT_TYPE_PORT, &port_data, NULL))) {
        return status;
    }

    if (SX_STATUS_SUCCESS !=
        (status = sx_api_port_counter_clear_set(gh_sdk, port_data, false, SX_PORT_CNTR_GRP_ALL))) {
        SX_LOG_ERR("Failed to clear all port counters - %s.\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

sai_status_t mlnx_port_log_set(sx_verbosity_level_t level)
{
    LOG_VAR_NAME(__MODULE__) = level;

    if (gh_sdk) {
        return sdk_to_sai(sx_api_port_log_verbosity_level_set(gh_sdk, SX_LOG_VERBOSITY_BOTH, level, level));
    } else {
        return SAI_STATUS_SUCCESS;
    }
}

/*
 * Get index of port configuration in port qos db
 *
 * Arguments:
 *    [in]  log_port_id - logical port id
 *    [out] index       - index of the port in qos db
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS
 *    SAI_STATUS_FAILURE
 *
 */
/* DB read lock is needed */
sai_status_t mlnx_port_idx_by_log_id(sx_port_log_id_t log_port_id, uint32_t *index)
{
    mlnx_port_config_t *port;
    uint32_t            ii = 0;

    assert(index != NULL);

    mlnx_port_foreach(port, ii) {
        if (log_port_id == port->logical) {
            *index = ii;
            return SAI_STATUS_SUCCESS;
        }
    }

    SX_LOG_ERR("Port index not found in DB by log id 0x%x\n", log_port_id);
    return SAI_STATUS_INVALID_PORT_NUMBER;
}

/* DB read lock is needed */
sai_status_t mlnx_port_idx_by_obj_id(sai_object_id_t obj_id, uint32_t *index)
{
    mlnx_port_config_t *port;
    uint32_t            ii = 0;

    assert(index != NULL);

    mlnx_port_foreach(port, ii) {
        if (obj_id == port->saiport) {
            *index = ii;
            return SAI_STATUS_SUCCESS;
        }
    }

    SX_LOG_ERR("Port index not found in DB by obj id %" PRIx64 "\n", obj_id);
    return SAI_STATUS_INVALID_PORT_NUMBER;
}

/* DB read lock is needed */
sai_status_t mlnx_port_by_log_id(sx_port_log_id_t log_id, mlnx_port_config_t **port)
{
    mlnx_port_config_t *port_cfg;
    uint32_t            ii;

    assert(port != NULL);

    mlnx_port_foreach(port_cfg, ii) {
        if (port_cfg->logical == log_id) {
            *port = port_cfg;
            return SAI_STATUS_SUCCESS;
        }
    }

    SX_LOG_ERR("Failed lookup port config by log id 0x%x\n", log_id);
    return SAI_STATUS_INVALID_PORT_NUMBER;
}

sai_status_t mlnx_port_by_obj_id(sai_object_id_t obj_id, mlnx_port_config_t **port)
{
    mlnx_port_config_t *port_cfg;
    uint32_t            ii;

    assert(port != NULL);

    mlnx_port_foreach(port_cfg, ii) {
        if (port_cfg->saiport == obj_id) {
            *port = port_cfg;
            return SAI_STATUS_SUCCESS;
        }
    }

    SX_LOG_ERR("Failed lookup port config by object id %" PRIx64 "\n", obj_id);
    return SAI_STATUS_INVALID_PORT_NUMBER;
}

uint32_t mlnx_port_idx_get(const mlnx_port_config_t *port)
{
    return port->index;
}

bool mlnx_port_is_phy(const mlnx_port_config_t *port)
{
    return SX_PORT_TYPE_ID_GET(port->logical) == SX_PORT_TYPE_NETWORK;
}

bool mlnx_port_is_virt(const mlnx_port_config_t *port)
{
    return SX_PORT_TYPE_ID_GET(port->logical) == SX_PORT_TYPE_VPORT;
}

bool mlnx_port_is_lag(const mlnx_port_config_t *port)
{
    return SX_PORT_TYPE_ID_GET(port->logical) == SX_PORT_TYPE_LAG;
}

bool mlnx_port_is_lag_member(const mlnx_port_config_t *port)
{
    return !!port->lag_id;
}

bool mlnx_log_port_is_cpu(sx_port_log_id_t log_id)
{
    return log_id == CPU_PORT;
}

const char * mlnx_port_type_str(const mlnx_port_config_t *port)
{
    if (mlnx_port_is_lag(port)) {
        return "lag";
    }

    return "port";
}

/** Ingress buffer profiles for port [sai_object_list_t]
 *  There can be up to SAI_SWITCH_ATTR_INGRESS_BUFFER_POOL_NUM profiles */
static sai_status_t mlnx_port_ingress_buffer_profile_list_get(_In_ const sai_object_key_t   *key,
                                                              _Inout_ sai_attribute_value_t *value,
                                                              _In_ uint32_t                  attr_index,
                                                              _Inout_ vendor_cache_t        *cache,
                                                              void                          *arg)
{
    return mlnx_buffer_port_profile_list_get(key->object_id, value, true);
}

/** Ingress buffer profiles for port [sai_object_list_t]
 *  There can be up to SAI_SWITCH_ATTR_INGRESS_BUFFER_POOL_NUM profiles */
static sai_status_t mlnx_port_ingress_buffer_profile_list_set(_In_ const sai_object_key_t      *key,
                                                              _In_ const sai_attribute_value_t *value,
                                                              void                             *arg)
{
    return mlnx_buffer_port_profile_list_set(key->object_id, value, true);
}

/** Egress buffer profiles for port [sai_object_list_t]
 *  There can be up to SAI_SWITCH_ATTR_EGRESS_BUFFER_POOL_NUM profiles */
static sai_status_t mlnx_port_egress_buffer_profile_list_get(_In_ const sai_object_key_t   *key,
                                                             _Inout_ sai_attribute_value_t *value,
                                                             _In_ uint32_t                  attr_index,
                                                             _Inout_ vendor_cache_t        *cache,
                                                             void                          *arg)
{
    return mlnx_buffer_port_profile_list_get(key->object_id, value, false);
}

/** Egress buffer profiles for port [sai_object_list_t]
 *  There can be up to SAI_SWITCH_ATTR_EGRESS_BUFFER_POOL_NUM profiles */
static sai_status_t mlnx_port_egress_buffer_profile_list_set(_In_ const sai_object_key_t      *key,
                                                             _In_ const sai_attribute_value_t *value,
                                                             void                             *arg)
{
    return mlnx_buffer_port_profile_list_set(key->object_id, value, false);
}

static sai_status_t mlnx_port_storm_control_policer_attr_set(_In_ const sai_object_key_t      *key,
                                                             _In_ const sai_attribute_value_t *value,
                                                             _In_ void                        *arg)
{
    sai_status_t             status;
    mlnx_policer_bind_params bind_params;
    mlnx_port_policer_type   policer_function;

    SX_LOG_ENTER();
    policer_function = (mlnx_port_policer_type)arg;
    if (policer_function >= MLNX_PORT_POLICER_TYPE_MAX) {
        SX_LOG_ERR("Invalid policer type:%d passed for port:%" PRIx64 "\n", policer_function, key->object_id);
        SX_LOG_EXIT();
        return SAI_STATUS_INVALID_PARAMETER;
    }

    bind_params.port_policer_type = policer_function;

    if (SAI_NULL_OBJECT_ID == value->oid) {
        status = mlnx_sai_unbind_policer(key->object_id, &bind_params);
    } else {
        status = mlnx_sai_bind_policer(key->object_id, value->oid, &bind_params);
    }
    SX_LOG_EXIT();
    return status;
}

static sai_status_t mlnx_port_storm_control_policer_attr_get(_In_ const sai_object_key_t   *key,
                                                             _Inout_ sai_attribute_value_t *value,
                                                             _In_ uint32_t                  attr_index,
                                                             _Inout_ vendor_cache_t        *cache,
                                                             _In_ void                     *arg)
{
    sai_status_t             status;
    sx_port_log_id_t         port_id;
    uint32_t                 port_db_ind = 0;
    sai_object_id_t          sai_policer;
    mlnx_port_policer_type   policer_function;
    mlnx_policer_db_entry_t *policer_db_entry = NULL;

    SX_LOG_ENTER();
    if (SAI_STATUS_SUCCESS != (status = mlnx_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id, NULL))) {
        SX_LOG_EXIT();
        return status;
    }
    policer_function = (mlnx_port_policer_type)arg;
    if (policer_function >= MLNX_PORT_POLICER_TYPE_MAX) {
        SX_LOG_ERR("Invalid policer type:%d passed for port:%" PRIx64 "\n", policer_function, key->object_id);
        SX_LOG_EXIT();
        return SAI_STATUS_INVALID_PARAMETER;
    }
    for (port_db_ind = 0; port_db_ind < MAX_PORTS; port_db_ind++) {
        if (key->object_id == g_sai_db_ptr->ports_db[port_db_ind].saiport) {
            break;
        }
    }
    if (MAX_PORTS == port_db_ind) {
        SX_LOG_ERR("Internal error: No entry found in port_db for sai port object:%" PRIx64 ".\n", key->object_id);
        SX_LOG_EXIT();
        return SAI_STATUS_INVALID_PARAMETER;
    }

    sai_policer = g_sai_db_ptr->ports_db[port_db_ind].port_policers[policer_function];

    if (SAI_NULL_OBJECT_ID == sai_policer) {
        SX_LOG_DBG("port:%" PRIx64 ", port_db[%d] contains NULL policer\n", key->object_id, policer_function);
        SX_LOG_EXIT();
        value->oid = sai_policer;
        return SAI_STATUS_SUCCESS;
    }
    if (SAI_STATUS_SUCCESS != (status = db_get_sai_policer_data(sai_policer, &policer_db_entry))) {
        SX_LOG_ERR("Failed to obtain db entry for sai_policer:%" PRIx64 "\n", sai_policer);
        SX_LOG_EXIT();
        return status;
    }
    value->oid = sai_policer;
    SX_LOG_EXIT();
    return status;
}

mlnx_port_config_t * mlnx_port_by_idx(uint8_t id)
{
    return &mlnx_ports_db[id];
}

mlnx_port_config_t * mlnx_port_by_local_id(uint8_t local_port)
{
    mlnx_port_config_t *port;
    uint32_t            ii;

    for (ii = 0; ii < MAX_PORTS; ii++) {
        port = &mlnx_ports_db[ii];

        if (port->port_map.local_port == local_port) {
            return port;
        }
    }

    assert(false);
    return NULL;
}

static sai_status_t mlnx_port_speed_convert_bitmap_to_capability(const sx_port_speed_t       speed_bitmap,
                                                                 sx_port_speed_capability_t* speed_capability)
{
    if (NULL == speed_capability) {
        SX_LOG_ERR("NULL pointer: Port Speed Capability");
        return SAI_STATUS_FAILURE;
    }

    memset(speed_capability, 0, sizeof(*speed_capability));
    if (speed_bitmap & 1) {
        speed_capability->mode_1GB_CX_SGMII = TRUE;
    }
    if (speed_bitmap & 1 << 1) {
        speed_capability->mode_1GB_KX = TRUE;
    }
    if (speed_bitmap & 1 << 2) {
        speed_capability->mode_10GB_CX4_XAUI = TRUE;
    }
    if (speed_bitmap & 1 << 3) {
        speed_capability->mode_10GB_KX4 = TRUE;
    }
    if (speed_bitmap & 1 << 4) {
        speed_capability->mode_10GB_KR = TRUE;
    }
    if (speed_bitmap & 1 << 5) {
        speed_capability->mode_20GB_KR2 = TRUE;
    }
    if (speed_bitmap & 1 << 6) {
        speed_capability->mode_40GB_CR4 = TRUE;
    }
    if (speed_bitmap & 1 << 7) {
        speed_capability->mode_40GB_KR4 = TRUE;
    }
    if (speed_bitmap & 1 << 8) {
        speed_capability->mode_56GB_KR4 = TRUE;
    }
    if (speed_bitmap & 1 << 9) {
        speed_capability->mode_56GB_KX4 = TRUE;
    }
    if (speed_bitmap & 1 << 12) {
        speed_capability->mode_10GB_CR = TRUE;
    }
    if (speed_bitmap & 1 << 13) {
        speed_capability->mode_10GB_SR = TRUE;
    }
    if (speed_bitmap & 1 << 14) {
        speed_capability->mode_10GB_ER_LR = TRUE;
    }
    if (speed_bitmap & 1 << 15) {
        speed_capability->mode_40GB_SR4 = TRUE;
    }
    if (speed_bitmap & 1 << 16) {
        speed_capability->mode_40GB_LR4_ER4 = TRUE;
    }
    if (speed_bitmap & 1 << 20) {
        speed_capability->mode_100GB_CR4 = TRUE;
    }
    if (speed_bitmap & 1 << 21) {
        speed_capability->mode_100GB_SR4 = TRUE;
    }
    if (speed_bitmap & 1 << 22) {
        speed_capability->mode_100GB_KR4 = TRUE;
    }
    if (speed_bitmap & 1 << 23) {
        speed_capability->mode_100GB_LR4_ER4 = TRUE;
    }
    if (speed_bitmap & 1 << 27) {
        speed_capability->mode_25GB_CR = TRUE;
    }
    if (speed_bitmap & 1 << 28) {
        speed_capability->mode_25GB_KR = TRUE;
    }
    if (speed_bitmap & 1 << 29) {
        speed_capability->mode_25GB_SR = TRUE;
    }
    if (speed_bitmap & 1 << 30) {
        speed_capability->mode_50GB_CR2 = TRUE;
    }
    if (speed_bitmap & 1 << 31) {
        speed_capability->mode_50GB_KR2 = TRUE;
    }
    if (speed_bitmap == 0xFFFFFFFF) {
        speed_capability->mode_auto = TRUE;
    }

    return SAI_STATUS_SUCCESS;
}

sai_status_t mlnx_port_config_init(mlnx_port_config_t *port)
{
    sx_port_admin_state_t      state = SX_PORT_ADMIN_STATUS_DOWN;
    sx_port_speed_capability_t admin_speed;
    sx_vlan_ports_t            vlan_port;
    sai_status_t               status;

    assert(port != NULL);

    if (mlnx_port_is_lag(port)) {
        state = SX_PORT_ADMIN_STATUS_UP;
    }

    memset(&vlan_port, 0, sizeof(vlan_port));
    vlan_port.log_port    = port->logical;
    vlan_port.is_untagged = true;

    memset(&admin_speed, 0, sizeof(admin_speed));

    if (mlnx_port_is_lag(port)) {
        admin_speed.mode_40GB_CR4     = true;
        admin_speed.mode_40GB_SR4     = true;
        admin_speed.mode_40GB_LR4_ER4 = true;
    } else {
        status = mlnx_port_speed_convert_bitmap_to_capability(port->port_speed, &admin_speed);
        if (SAI_ERR(status)) {
            SX_LOG_ERR("Failed to convert port %x speed %d\n", port->logical, port->port_speed);
            return status;
        }
    }

    port->start_queues_index = port->index * (MAX_ETS_TC + 1);

    /* Configure regular (network) port type only */
    if (mlnx_port_is_phy(port)) {
        status = sx_api_port_swid_bind_set(gh_sdk, port->logical, DEFAULT_ETH_SWID);
        if (SX_ERR(status)) {
            SX_LOG_ERR("Port swid bind %x failed - %s\n", port->logical, SX_STATUS_MSG(status));
            return sdk_to_sai(status);
        }

        status = sx_api_port_init_set(gh_sdk, port->logical);
        if (SX_ERR(status)) {
            SX_LOG_ERR("Port init set %x failed - %s\n", port->logical, SX_STATUS_MSG(status));
            return sdk_to_sai(status);
        }

        status = sx_api_port_phys_loopback_set(gh_sdk, port->logical, SX_PORT_PHYS_LOOPBACK_DISABLE);
        if (SX_ERR(status)) {
            SX_LOG_ERR("Port phys loopback set %x failed - %s\n", port->logical, SX_STATUS_MSG(status));
            return sdk_to_sai(status);
        }
    }

    status = sx_api_mstp_inst_port_state_set(gh_sdk, DEFAULT_ETH_SWID, mlnx_stp_get_default_stp(),
                                             port->logical, SX_MSTP_INST_PORT_STATE_FORWARDING);
    if (SX_ERR(status)) {
        SX_LOG_ERR("%s\n", SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    status = sx_api_port_speed_admin_set(gh_sdk, port->logical, &admin_speed);
    if (SX_ERR(status)) {
        SX_LOG_ERR("Port admin speed set %x failed - %s\n", port->logical, SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    status = sx_api_port_state_set(gh_sdk, port->logical, state);
    if (SX_ERR(status)) {
        SX_LOG_ERR("Port state set %x failed - %s\n", port->logical, SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    status = sx_api_vlan_port_pvid_set(gh_sdk, SX_ACCESS_CMD_ADD, port->logical, DEFAULT_VLAN);
    if (SX_ERR(status)) {
        SX_LOG_ERR("port pvid set %x failed - %s\n", port->logical, SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    status = sx_api_vlan_ports_set(gh_sdk, SX_ACCESS_CMD_ADD, DEFAULT_ETH_SWID,
                                   DEFAULT_VLAN, &vlan_port, 1);
    if (SX_ERR(status)) {
        SX_LOG_ERR("Port add port %x to vlan %u failed - %s\n", port->logical, DEFAULT_VLAN, SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    mlnx_vlan_port_set(DEFAULT_VLAN, port, true);

    status = sx_api_vlan_port_ingr_filter_set(gh_sdk, port->logical, SX_INGR_FILTER_ENABLE);
    if (SX_ERR(status)) {
        SX_LOG_ERR("Port ingress filter set %x failed - %s\n", port->logical, SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    port->is_present = true;

    if (!mlnx_port_is_virt(port)) {
        uint32_t ii;

        status = sx_api_cos_port_trust_set(gh_sdk, port->logical, SX_COS_TRUST_LEVEL_PORT);
        if (SX_ERR(status)) {
            SX_LOG_ERR("Port trust level set %x failed - %s\n", port->logical, SX_STATUS_MSG(status));
            return sdk_to_sai(status);
        }

        status = sx_api_port_global_fc_enable_set(gh_sdk, port->logical,
                                                  SX_PORT_FLOW_CTRL_MODE_TX_DIS_RX_DIS);
        if (SX_ERR(status)) {
            SX_LOG_ERR("Failed to init port global flow control - %s\n", SX_STATUS_MSG(status));
            return sdk_to_sai(status);
        }

        for (ii = 0; ii < MLNX_QOS_MAP_TYPES_MAX; ii++) {
            status = mlnx_port_qos_map_apply(port->saiport, SAI_NULL_OBJECT_ID, ii);
            if (SAI_ERR(status)) {
                return status;
            }
        }

        status = mlnx_sched_group_port_init(port);
        if (SAI_ERR(status)) {
            SX_LOG_ERR("Failed init SAI scheduler groups for log port 0x%x\n", port->logical);
            return status;
        }
    }

    return SAI_STATUS_SUCCESS;
}

sai_status_t mlnx_port_add(mlnx_port_config_t *port)
{
    sai_status_t status;

    status = mlnx_port_config_init(port);
    if (SAI_ERR(status)) {
        return status;
    }

    status = mlnx_acl_port_lag_event_handle(port, ACL_EVENT_TYPE_PORT_LAG_ADD);
    if (SAI_ERR(status)) {
        return status;
    }

    return SAI_STATUS_SUCCESS;
}

static void mlnx_port_reset_buffer_refs(uint32_t *buff_refs, uint32_t count)
{
    uint32_t ii;

    for (ii = 0; ii < count; ii++) {
        buff_refs[ii] = SENTINEL_BUFFER_DB_ENTRY_INDEX;
    }
}

sai_status_t mlnx_port_config_uninit(mlnx_port_config_t *port)
{
    mlnx_policer_bind_params bind_params;
    sx_vlan_ports_t          vlan_port;
    sx_port_mapping_t        port_map;
    sai_status_t             status;
    sx_vid_t                 pvid;

    /* Reset Policer's */
    bind_params.port_policer_type = MLNX_PORT_POLICER_TYPE_FLOOD_INDEX;
    status                        = mlnx_sai_unbind_policer_from_port(port->saiport, &bind_params);
    if (SAI_ERR(status)) {
        return status;
    }
    bind_params.port_policer_type = MLNX_PORT_POLICER_TYPE_BROADCAST_INDEX;
    status                        = mlnx_sai_unbind_policer_from_port(port->saiport, &bind_params);
    if (SAI_ERR(status)) {
        return status;
    }
    bind_params.port_policer_type = MLNX_PORT_POLICER_TYPE_MULTICAST_INDEX;
    status                        = mlnx_sai_unbind_policer_from_port(port->saiport, &bind_params);
    if (SAI_ERR(status)) {
        return status;
    }
    bind_params.port_policer_type = MLNX_PORT_POLICER_TYPE_REGULAR_INDEX;
    status                        = mlnx_sai_unbind_policer_from_port(port->saiport, &bind_params);
    if (SAI_ERR(status)) {
        return status;
    }

    memset(&vlan_port, 0, sizeof(vlan_port));
    vlan_port.log_port    = port->logical;
    vlan_port.is_untagged = true;

    status = sx_api_vlan_port_pvid_get(gh_sdk, port->logical, &pvid);
    if (!SX_ERR(status)) {
        status = sx_api_vlan_port_pvid_set(gh_sdk, SX_ACCESS_CMD_DELETE, port->logical, pvid);
        if (SX_ERR(status) && (status != SX_STATUS_ENTRY_NOT_FOUND)) {
            SX_LOG_ERR("Port pvid un-set %x failed - %s\n", port->logical, SX_STATUS_MSG(status));
            return sdk_to_sai(status);
        }
    }

    status = sx_api_vlan_ports_set(gh_sdk, SX_ACCESS_CMD_DELETE, DEFAULT_ETH_SWID,
                                   DEFAULT_VLAN, &vlan_port, 1);
    if (SX_ERR(status) && (status != SX_STATUS_ENTRY_NOT_FOUND)) {
        SX_LOG_ERR("Port del port %x from vlan %u failed - %s\n", port->logical, DEFAULT_VLAN, SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

/*    status = sx_api_rstp_port_state_set(gh_sdk, port->logical, SX_MSTP_INST_PORT_STATE_DISCARDING); */
    status = sx_api_mstp_inst_port_state_set(gh_sdk, DEFAULT_ETH_SWID, mlnx_stp_get_default_stp(),
                                             port->logical, SX_MSTP_INST_PORT_STATE_FORWARDING);
    if (SX_ERR(status)) {
        SX_LOG_ERR("Port rstp state set %x failed - %s\n", port->logical, SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    if (mlnx_port_is_phy(port)) {
        status = sx_api_port_deinit_set(gh_sdk, port->logical);
        if (SX_ERR(status)) {
            SX_LOG_ERR("Port de-init set %x failed - %s\n", port->logical, SX_STATUS_MSG(status));
            return sdk_to_sai(status);
        }

        status = sx_api_port_swid_bind_set(gh_sdk, port->logical, SX_SWID_ID_DISABLED);
        if (SX_ERR(status)) {
            SX_LOG_ERR("Port swid bind %x failed - %s\n", port->logical, SX_STATUS_MSG(status));
            return sdk_to_sai(status);
        }

        port_map.mapping_mode = SX_PORT_MAPPING_MODE_DISABLE;
        port_map.local_port   = port->port_map.local_port;
        port_map.module_port  = port->module;
        port_map.config_hw    = FALSE;
        port_map.lane_bmap    = 0x0;

        status = sx_api_port_mapping_set(gh_sdk, &port->logical, &port_map, 1);
        if (SX_ERR(status)) {
            SX_LOG_ERR("Set disable port mapping %x failed - %s\n", port->logical, SX_STATUS_MSG(status));
            return sdk_to_sai(status);
        }
    }

    port->is_present = false;

    if (!SAI_ERR(status)) {
        uint32_t                *buff_refs  = NULL;
        uint32_t                 buff_count = 0;
        mlnx_qos_queue_config_t *queue;
        uint32_t                 ii;

        /* Reset QoS */
        memset(&port->sched_hierarchy, 0, sizeof(port->sched_hierarchy));
        memset(port->qos_maps, 0, sizeof(port->qos_maps));
        port->scheduler_id = SAI_NULL_OBJECT_ID;

        port->internal_ingress_samplepacket_obj_idx = MLNX_INVALID_SAMPLEPACKET_SESSION;
        port->internal_egress_samplepacket_obj_idx  = MLNX_INVALID_SAMPLEPACKET_SESSION;

        port->wred_id = SAI_NULL_OBJECT_ID;

        if (mlnx_port_is_phy(port)) {
            uint32_t port_index = mlnx_port_idx_get(port);

            /* Reset buffers */
            /* PG Buff's */
            status = mlnx_sai_get_port_buffer_index_array(port_index, PORT_BUFF_TYPE_PG, &buff_refs);
            if (SAI_ERR(status)) {
                return status;
            }
            buff_count = mlnx_sai_get_buffer_resource_limits()->num_port_pg_buff;
            mlnx_port_reset_buffer_refs(buff_refs, buff_count);

            /* Ingress Buff's */
            status = mlnx_sai_get_port_buffer_index_array(port_index, PORT_BUFF_TYPE_INGRESS, &buff_refs);
            if (SAI_ERR(status)) {
                return status;
            }
            buff_count = mlnx_sai_get_buffer_resource_limits()->num_ingress_pools;
            mlnx_port_reset_buffer_refs(buff_refs, buff_count);

            /* Egress Buff's */
            status = mlnx_sai_get_port_buffer_index_array(port_index, PORT_BUFF_TYPE_EGRESS, &buff_refs);
            if (SAI_ERR(status)) {
                return status;
            }
            buff_count = mlnx_sai_get_buffer_resource_limits()->num_egress_pools;
            mlnx_port_reset_buffer_refs(buff_refs, buff_count);
        }

        /* Reset port's queues */
        port_queues_foreach(port, queue, ii) {
            queue->buffer_id = SAI_NULL_OBJECT_ID;
            queue->wred_id   = SAI_NULL_OBJECT_ID;
        }
    }

    return status;
}

sai_status_t mlnx_port_del(mlnx_port_config_t *port)
{
    sai_status_t status;

    status = sx_api_port_state_set(gh_sdk, port->logical, SX_PORT_ADMIN_STATUS_DOWN);
    if (SX_ERR(status)) {
        SX_LOG_ERR("Set port %x down failed - %s\n", port->logical, SX_STATUS_MSG(status));
        return sdk_to_sai(status);
    }

    acl_global_lock();
    status = mlnx_acl_port_lag_event_handle(port, ACL_EVENT_TYPE_PORT_LAG_DEL);
    acl_global_unlock();
    if (SAI_ERR(status)) {
        return status;
    }

    status = mlnx_port_config_uninit(port);
    if (SAI_ERR(status)) {
        SX_LOG_ERR("Failed un-init port %x\n", port->logical);
        return status;
    }

    return SAI_STATUS_SUCCESS;
}

#define SPAN_MAX_COUNT \
    (g_resource_limits.span_session_id_max_internal + g_resource_limits.span_session_id_max_external)

sai_status_t mlnx_port_in_use_check(const mlnx_port_config_t *port)
{
    uint32_t                        span_count = SPAN_MAX_COUNT;
    sx_span_session_id_t           *span_sessions;
    sx_span_analyzer_port_params_t *span_params;
    sx_status_t                     sx_status;

    if (port->lag_id) {
        SX_LOG_ERR("Failed remove port oid %" PRIx64 " - is a LAG member\n", port->saiport);
        return SAI_STATUS_OBJECT_IN_USE;
    }
    if (port->vlans) {
        SX_LOG_ERR("Failed remove port oid %" PRIx64 " - is a VLAN member\n", port->saiport);
        return SAI_STATUS_OBJECT_IN_USE;
    }
    if (port->rifs) {
        SX_LOG_ERR("Failed remove port oid %" PRIx64 " - is a router interface\n", port->saiport);
        return SAI_STATUS_OBJECT_IN_USE;
    }
    if (port->fdbs) {
        SX_LOG_ERR("Failed remove port oid %" PRIx64 " - is in FDB action\n", port->saiport);
        return SAI_STATUS_OBJECT_IN_USE;
    }

    if (mlnx_acl_is_port_lag_used(port)) {
        SX_LOG_ERR("Failed remove port oid %" PRIx64 " - is in ACL\n", port->saiport);
        return SAI_STATUS_OBJECT_IN_USE;
    }

    if (mlnx_port_is_lag(port)) {
        return SAI_STATUS_SUCCESS;
    }

    span_sessions = malloc(sizeof(sx_span_session_id_t) * SPAN_MAX_COUNT);
    span_params   = malloc(sizeof(sx_span_analyzer_port_params_t) * SPAN_MAX_COUNT);

    sx_status = sx_api_span_analyzer_get(gh_sdk, port->logical, span_params, span_sessions, &span_count);
    free(span_sessions);
    free(span_params);

    if (sx_status != SX_STATUS_ENTRY_NOT_FOUND) {
        SX_LOG_ERR("Failed remove port oid %" PRIx64 " - is Mirror analyzer(monitor) port\n", port->saiport);
        return SAI_STATUS_OBJECT_IN_USE;
    }

    return SAI_STATUS_SUCCESS;
}

static inline uint32_t lane2module(uint32_t lane)
{
    return lane / MAX_LANES;
}

static inline uint32_t sai_lane2phy_lane(uint32_t lane)
{
    return (lane - lane2module(lane) * MAX_LANES);
}

const sai_port_api_t mlnx_port_api = {
    mlnx_set_port_attribute,
    mlnx_get_port_attribute,
    mlnx_get_port_stats,
    mlnx_clear_port_stats,
    mlnx_clear_port_all_stats
};

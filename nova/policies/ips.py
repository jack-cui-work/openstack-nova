# Copyright 2016 Cloudbase Solutions Srl
# All Rights Reserved.
#
#    Licensed under the Apache License, Version 2.0 (the "License"); you may
#    not use this file except in compliance with the License. You may obtain
#    a copy of the License at
#
#         http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
#    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
#    License for the specific language governing permissions and limitations
#    under the License.

from oslo_policy import policy

from nova.policies import base


POLICY_ROOT = 'os_compute_api:ips:%s'


ips_policies = [
    policy.DocumentedRuleDefault(
        name=POLICY_ROOT % 'show',
        check_str=base.PROJECT_READER_OR_ADMIN,
        description="Show IP addresses details for a network label of a "
        " server",
        operations=[
            {
                'method': 'GET',
                'path': '/servers/{server_id}/ips/{network_label}'
            }
        ],
        scope_types=['project']),
    policy.DocumentedRuleDefault(
        name=POLICY_ROOT % 'index',
        check_str=base.PROJECT_READER_OR_ADMIN,
        description="List IP addresses that are assigned to a server",
        operations=[
            {
                'method': 'GET',
                'path': '/servers/{server_id}/ips'
            }
        ],
        scope_types=['project']),
]


def list_rules():
    return ips_policies

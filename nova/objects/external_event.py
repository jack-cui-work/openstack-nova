#    Copyright 2014 Red Hat, Inc.
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

from nova.objects import base as obj_base
from nova.objects import fields

EVENT_NAMES = [
    # Network has changed for this instance, rebuild info_cache
    'network-changed',

    # VIF plugging notifications, tag is port_id
    'network-vif-plugged',
    'network-vif-unplugged',
    'network-vif-deleted',

    # Volume was extended for this instance, tag is volume_id
    'volume-extended',

    # Power state has changed for this instance
    'power-update',

    # Accelerator Request got bound, tag is ARQ uuid.
    # Sent when an ARQ for an instance has been bound or failed to bind.
    'accelerator-request-bound',

    # re-image operation has completed from cinder side
    'volume-reimaged',
]

EVENT_STATUSES = ['failed', 'completed', 'in-progress']

# Possible tag values for the power-update event.
POWER_ON = 'POWER_ON'
POWER_OFF = 'POWER_OFF'


@obj_base.NovaObjectRegistry.register
class InstanceExternalEvent(obj_base.NovaObject):
    # Version 1.0: Initial version
    #              Supports network-changed and vif-plugged
    # Version 1.1: adds network-vif-deleted event
    # Version 1.2: adds volume-extended event
    # Version 1.3: adds power-update event
    # Version 1.4: adds accelerator-request-bound event
    # Version 1.5: adds volume-reimaged event
    VERSION = '1.5'

    fields = {
        'instance_uuid': fields.UUIDField(),
        'name': fields.EnumField(valid_values=EVENT_NAMES),
        'status': fields.EnumField(valid_values=EVENT_STATUSES),
        'tag': fields.StringField(nullable=True),
        'data': fields.DictOfStringsField(),
        }

    @staticmethod
    def make_key(name, tag=None):
        if tag is not None:
            return '%s-%s' % (name, tag)
        else:
            return name

    @property
    def key(self):
        return self.make_key(self.name, self.tag)

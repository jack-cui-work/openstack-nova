.. -*- rst -*-

===================================================================
 Servers with volume attachments (servers, os-volume\_attachments)
===================================================================

Attaches volumes that are created through the volume API to server
instances. Also, lists volume attachments for a server, shows
details for a volume attachment, and detaches a volume.

List volume attachments for an instance
=======================================

.. rest_method:: GET /servers/{server_id}/os-volume_attachments

List volume attachments for an instance.

Normal response codes: 200

Error response codes: unauthorized(401), forbidden(403), itemNotFound(404)

Request
-------

.. rest_parameters:: parameters.yaml

  - server_id: server_id_path
  - limit: limit_simple
  - offset: offset_simple

Response
--------

.. rest_parameters:: parameters.yaml

  - volumeAttachments: volumeAttachments
  - id: volume_attachment_id_resp
  - serverId: server_id
  - volumeId: volumeId_resp
  - device: attachment_device_resp
  - tag: device_tag_bdm_attachment_resp
  - delete_on_termination: delete_on_termination_attachments_resp
  - attachment_id: attachment_volume_id_resp
  - bdm_uuid: attachment_bdm_id_resp

**Example List volume attachments for an instance: JSON response**

.. literalinclude:: ../../doc/api_samples/os-volumes/list-volume-attachments-resp.json
   :language: javascript

**Example List tagged volume attachments for an instance (v2.89): JSON response**

.. literalinclude:: ../../doc/api_samples/os-volumes/v2.89/list-volume-attachments-resp.json
   :language: javascript

Attach a volume to an instance
==============================

.. rest_method:: POST /servers/{server_id}/os-volume_attachments

Attach a volume to an instance.

Normal response codes: 200

Error response codes: badRequest(400), unauthorized(401), forbidden(403), itemNotFound(404), conflict(409)

.. note:: From v2.20 attach a volume to an instance in SHELVED or SHELVED_OFFLOADED
          state is allowed.

.. note:: From v2.60, attaching a multiattach volume to multiple instances is
          supported for instances that are not SHELVED_OFFLOADED. The ability
          to actually support a multiattach volume depends on the volume type
          and compute hosting the instance.

.. note:: This is an asynchronous API, callers should poll the status and list
          of attachments of the volume within the volume API to determine when
          the attachment has completed successfully.

Request
-------

.. rest_parameters:: parameters.yaml

  - server_id: server_id_path
  - volumeAttachment: volumeAttachment_post
  - volumeId: volumeId
  - device: device
  - tag: device_tag_bdm_attachment
  - delete_on_termination: delete_on_termination_attachments_req

**Example Attach a volume to an instance: JSON request**

.. literalinclude:: ../../doc/api_samples/os-volumes/attach-volume-to-server-req.json
   :language: javascript

**Example Attach a volume to an instance and tag it (v2.49): JSON request**

.. literalinclude:: ../../doc/api_samples/os-volumes/v2.49/attach-volume-to-server-req.json
   :language: javascript

**Example Attach a volume to an instance with "delete_on_termination" (v2.79): JSON request**

.. literalinclude:: ../../doc/api_samples/os-volumes/v2.79/attach-volume-to-server-req.json
   :language: javascript

Response
--------

.. rest_parameters:: parameters.yaml

  - volumeAttachment: volumeAttachment
  - device: device_resp
  - id: attachment_id_resp
  - serverId: server_id
  - volumeId: volumeId_resp
  - tag: device_tag_bdm_attachment_resp
  - delete_on_termination: delete_on_termination_attachments_resp

**Example Attach a volume to an instance: JSON response**

.. literalinclude:: ../../doc/api_samples/os-volumes/attach-volume-to-server-resp.json
   :language: javascript

**Example Attach a tagged volume to an instance (v2.70): JSON response**

.. literalinclude:: ../../doc/api_samples/os-volumes/v2.70/attach-volume-to-server-resp.json
   :language: javascript

**Example Attach a volume with "delete_on_termination" (v2.79): JSON response**

.. literalinclude:: ../../doc/api_samples/os-volumes/v2.79/attach-volume-to-server-resp.json
   :language: javascript

Show a detail of a volume attachment
====================================

.. rest_method:: GET /servers/{server_id}/os-volume_attachments/{volume_id}

Show a detail of a volume attachment.

Normal response codes: 200

Error response codes: unauthorized(401), forbidden(403), itemNotFound(404)

Request
-------

.. rest_parameters:: parameters.yaml

  - server_id: server_id_path
  - volume_id: volume_id_attached_path

Response
--------

.. rest_parameters:: parameters.yaml

  - volumeAttachment: volumeAttachment
  - id: volume_attachment_id_resp
  - serverId: server_id
  - volumeId: volumeId_resp
  - device: attachment_device_resp
  - tag: device_tag_bdm_attachment_resp
  - delete_on_termination: delete_on_termination_attachments_resp
  - attachment_id: attachment_volume_id_resp
  - bdm_uuid: attachment_bdm_id_resp

**Example Show a detail of a volume attachment: JSON response**

.. literalinclude:: ../../doc/api_samples/os-volumes/volume-attachment-detail-resp.json
   :language: javascript

**Example Show a detail of a tagged volume attachment (v2.89): JSON response**

.. literalinclude:: ../../doc/api_samples/os-volumes/v2.89/volume-attachment-detail-resp.json
   :language: javascript

Update a volume attachment
==========================

.. rest_method:: PUT /servers/{server_id}/os-volume_attachments/{volume_id}

Update a volume attachment.

.. note:: This action only valid when the server is in ACTIVE, PAUSED and RESIZED state,
          or a conflict(409) error will be returned.

.. warning:: When updating volumeId, this API is typically meant to
             only be used as part of a larger orchestrated volume
             migration operation initiated in the block storage
             service via the ``os-retype`` or ``os-migrate_volume``
             volume actions. Direct usage of this API to update
             volumeId is not recommended and may result in needing to
             hard reboot the server to update details within the guest
             such as block storage serial IDs. Furthermore, updating
             volumeId via this API is only implemented by `certain
             compute drivers`_.

.. _certain compute drivers: https://docs.openstack.org/nova/latest/user/support-matrix.html#operation_swap_volume

Policy default role is 'rule:system_admin_or_owner', its scope is
[system, project], which allow project members or system admins to
change the fields of an attached volume of a server. Policy defaults
enable only users with the administrative role to change ``volumeId``
via this operation. Cloud providers can change these permissions
through the ``policy.json`` file.

Updating, or what is commonly referred to as "swapping", volume attachments
with volumes that have more than one read/write attachment, is not supported.

Normal response codes: 202

Error response codes: badRequest(400), unauthorized(401), forbidden(403), itemNotFound(404), conflict(409)

Request
-------

.. rest_parameters:: parameters.yaml

  - server_id: server_id_path
  - volume_id: volume_id_swap_src
  - volumeAttachment: volumeAttachment_put
  - volumeId: volumeId_swap
  - delete_on_termination: delete_on_termination_put_req
  - device: attachment_device_put_req
  - serverId: attachment_server_id_put_req
  - tag: device_tag_bdm_attachment_put_req
  - id: attachment_id_put_req

.. note:: Other than ``volumeId``, as of v2.85 only
          ``delete_on_termination`` may be changed from the current
          value.

**Example Update a volume attachment (v2.85): JSON request**

.. literalinclude:: ../../doc/api_samples/os-volumes/v2.85/update-volume-attachment-delete-flag-req.json
   :language: javascript

Response
--------

No body is returned on successful request.

Detach a volume from an instance
================================

.. rest_method:: DELETE /servers/{server_id}/os-volume_attachments/{volume_id}

Detach a volume from an instance.

Normal response codes: 202

Error response codes: badRequest(400), unauthorized(401), forbidden(403), itemNotFound(404), conflict(409)

.. note:: From v2.20 detach a volume from an instance in SHELVED or SHELVED_OFFLOADED
          state is allowed.

.. note:: This is an asynchronous API, callers should poll the list
          of volume attachments provided by ``GET
          /servers/{server_id}/os-volume_attachments`` to determine when the
          detachment of the volume has completed successfully.

Request
-------

.. rest_parameters:: parameters.yaml

  - server_id: server_id_path
  - volume_id: volume_id_to_detach_path

Response
--------

No body is returned on successful request.

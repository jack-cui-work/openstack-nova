# Copyright 2010 United States Government as represented by the
# Administrator of the National Aeronautics and Space Administration.
# Copyright 2010 OpenStack Foundation
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

"""
Test WSGI basics and provide some helper functions for other WSGI tests.
"""

import sys
from unittest import mock

import routes
import webob

from nova.api.openstack import wsgi_app
from nova.api import wsgi
from nova import test
from nova import utils


class Test(test.NoDBTestCase):

    def test_router(self):

        class Application(wsgi.Application):
            """Test application to call from router."""

            def __call__(self, environ, start_response):
                start_response("200", [])
                return ['Router result']

        class Router(wsgi.Router):
            """Test router."""

            def __init__(self):
                mapper = routes.Mapper()
                mapper.connect("/test", controller=Application())
                super(Router, self).__init__(mapper)

        result = webob.Request.blank('/test').get_response(Router())
        self.assertEqual(result.body, "Router result")
        result = webob.Request.blank('/bad').get_response(Router())
        self.assertNotEqual(result.body, "Router result")

    @mock.patch('nova.api.openstack.wsgi_app._setup_service', new=mock.Mock())
    @mock.patch('paste.deploy.loadapp', new=mock.Mock())
    def test_init_application_passes_sys_argv_to_config(self):

        with utils.temporary_mutation(sys, argv=mock.sentinel.argv):
            with mock.patch('nova.config.parse_args') as mock_parse_args:
                wsgi_app.init_application('test-app')
                mock_parse_args.assert_called_once_with(
                    mock.sentinel.argv,
                    default_config_files=[
                        '/etc/nova/api-paste.ini', '/etc/nova/nova.conf'])

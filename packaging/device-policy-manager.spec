Name:    device-policy-manager
Version: 0.0.1
Release: 0
License: Apache-2.0
Source0: %{name}-%{version}.tar.gz
Summary: Tizen Device Policy Manager
Group:   Security/Other
BuildRequires: gcc
BuildRequires: cmake
BuildRequires: pam-devel
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(libxml-2.0)
BuildRequires: pkgconfig(sqlite3)
BuildRequires: pkgconfig(bundle)
BuildRequires: pkgconfig(appsvc)
BuildRequires: pkgconfig(syspopup-caller)
BuildRequires: pkgconfig(deviced)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(vconf-internal-keys)
BuildRequires: pkgconfig(libsmack)
BuildRequires: pkgconfig(libtzplatform-config)

%description
The device-policy-manager package provides a daemon which is responsible for
managing device policies.

%files
%manifest device-policy-manager.manifest
%defattr(644,root,root,755)
%attr(755,root,root) %{_bindir}/device-policy-manager
%dir %{TZ_SYS_DATA}/dpm
%dir %{TZ_SYS_ETC}/dpm/policy
%config %{TZ_SYS_ETC}/dpm/policy/PolicyManifest.xml

%prep
%setup -q

%build
%{!?build_type:%define build_type "RELEASE"}

%if %{build_type} == "DEBUG" || %{build_type} == "PROFILING" || %{build_type} == "CCOV"
	CFLAGS="$CFLAGS -Wp,-U_FORTIFY_SOURCE"
	CXXFLAGS="$CXXFLAGS -Wp,-U_FORTIFY_SOURCE"
%endif

%cmake . -DVERSION=%{version} \
	 -DCMAKE_BUILD_TYPE=%{build_type} \
	 -DSYSTEMD_UNIT_INSTALL_DIR=%{_unitdir} \
	 -DDATA_INSTALL_DIR=%{TZ_SYS_DATA}/dpm \
	 -DCONF_INSTALL_DIR=%{TZ_SYS_ETC}/dpm \
	 -DDB_INSTALL_DIR=%{TZ_SYS_DB} \
	 -DRUN_INSTALL_DIR=%{TZ_SYS_RUN} \
         -DAPP_INSTALL_PREFIX="%{TZ_SYS_RO_APP}" \
         -DAPP_SHARE_PACKAGES_DIR="%{TZ_SYS_RW_PACKAGES}" \
	 -DPAMD_INSTALL_DIR=/etc/pam.d

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%clean
rm -rf %{buildroot}

%post

%preun
%{_sbindir}/ldconfig

%postun

## Client Package #############################################################
%package -n libdpm
Summary: Tizen Device Policy Client library
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}
Requires(post): %{_sbindir}/ldconfig
Requires(postun): %{_sbindir}/ldconfig

%description -n libdpm
The libdpm package contains the libraries needed to run DPM client.

%post -n libdpm -p %{_sbindir}/ldconfig

%postun -n libdpm -p %{_sbindir}/ldconfig

%files -n libdpm
%defattr(644,root,root,755)
%attr(755,root,root) %{_libdir}/libdpm.so.%{version}
%{_libdir}/libdpm.so.0


## Devel Package ##############################################################
%package -n libdpm-devel
Summary: Libraries and header files for device policy client development
Group: Development/Libraries
Requires: libdpm = %{version}-%{release}

%description -n libdpm-devel
The libdpm-devel package includes the libraries and header files necessary for
developing the DPM client program.

%files -n libdpm-devel
%defattr(644,root,root,755)
%{_libdir}/libdpm.so
%{_includedir}/dpm
%{_libdir}/pkgconfig/dpm.pc

## Test Package ##############################################################
%package -n dpm-testcases
Summary: Device Policy Manager test cases
Group: Development/Libraries
Requires: libdpm = %{version}-%{release}

%description -n dpm-testcases
Testcases for device policy manager and device policy client

%files -n dpm-testcases
%defattr(644,root,root,755)
%attr(755,root,root) %{_bindir}/dpm-unit-tests
%attr(755,root,root) %{_bindir}/dpm-integration-tests
%attr(755,root,root) %{_bindir}/dpm-api-tests
%defattr(-,root,root,-)
%{TZ_SYS_DATA}/dpm/sample-policy.xml

## Tools Package ############################################################
%package -n org.tizen.ode-app
Summary: Tizen ODE application
Group: Security/Other
BuildRequires: pkgconfig(ui-gadget-1)
BuildRequires: pkgconfig(efl-extension)
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(capi-appfw-application)
BuildRequires: pkgconfig(evas)

%description -n org.tizen.ode-app
Tizen ODE application

%post -n org.tizen.ode-app
%{_sbindir}/ldconfig
%postun -n org.tizen.ode-app
%{_sbindir}/ldconfig

%define odeapp_home %{TZ_SYS_RO_APP}/org.tizen.ode-app

%files -n org.tizen.ode-app
%defattr(-,root,root,-)
%{odeapp_home}/bin/*
%{TZ_SYS_RW_PACKAGES}/org.tizen.ode-app.xml

## PAM Package ##############################################################
%package -n dpm-pam-zone
Summary: PAM for container policy in device policy manager
Group: Development/Libraries
Requires: systemd

%description -n dpm-pam-zone
PAM for container policy in device policy manager and CLI tool

%files -n dpm-pam-zone
%defattr(600,root,root,700)
%attr(700,root,root) %{_libdir}/security/pam_*.so
%attr(700,root,root) %{_sbindir}/nsattach
%config /etc/pam.d/*

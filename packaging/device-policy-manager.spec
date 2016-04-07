Name:    device-policy-manager
Version: 0.0.1
Release: 0
License: Apache-2.0
Source0: file://%{name}-%{version}.tar.gz
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
BuildRequires: pkgconfig(capi-base-common)
BuildRequires: pkgconfig(libsmack)
BuildRequires: pkgconfig(libtzplatform-config)
BuildRequires: pkgconfig(auth-fw-admin)

%description
The device-policy-manager package provides a daemon which is responsible for
managing device policies.

%files
%manifest device-policy-manager.manifest
%defattr(644,root,root,755)
%attr(755,root,root) %{_bindir}/device-policy-manager
%attr(700,root,root) %{_bindir}/factory-reset
%dir %{TZ_SYS_DATA}/dpm
%dir %{TZ_SYS_ETC}/dpm/policy
%config %{TZ_SYS_ETC}/dpm/policy/PolicyManifest.xml
%attr(700,root,root) %dir %{TZ_SYS_ETC}/dpm/zone
%attr(600,root,root) %config %{TZ_SYS_ETC}/dpm/zone/owner.xml

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
         -DSCRIPT_INSTALL_DIR=%{_scriptdir} \
         -DSYSTEMD_UNIT_INSTALL_DIR=%{_unitdir} \
         -DDATA_INSTALL_DIR=%{TZ_SYS_DATA}/dpm \
         -DCONF_INSTALL_DIR=%{TZ_SYS_ETC}/dpm \
         -DDB_INSTALL_DIR=%{TZ_SYS_DB} \
         -DRUN_INSTALL_DIR=%{TZ_SYS_RUN} \
         -DAPP_INSTALL_PREFIX="%{TZ_SYS_RO_APP}" \
         -DAPP_SHARE_PACKAGES_DIR="%{TZ_SYS_RO_PACKAGES}" \
         -DPAMD_INSTALL_DIR=/etc/pam.d

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/usr/apps/org.tizen.zone-setup-wizard/data
%make_install

%clean
rm -rf %{buildroot}

%preun

%postun

## Client Package #############################################################
%package -n libdpm
Summary: Tizen Device Policy Client library
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description -n libdpm
The libdpm package contains the libraries needed to run DPM client.

%post -n libdpm -p /sbin/ldconfig

%postun -n libdpm -p /sbin/ldconfig

%files -n libdpm
%manifest device-policy-manager.manifest
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
%manifest device-policy-manager.manifest
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
%package -n org.tizen.ode
Summary: Tizen ODE User Interface
Group: Security/Other
BuildRequires: pkgconfig(ui-gadget-1)
BuildRequires: pkgconfig(efl-extension)
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(capi-appfw-application)
BuildRequires: pkgconfig(evas)

%description -n org.tizen.ode
Tizen ODE User Interface for device policy management

%post -n org.tizen.ode
/sbin/ldconfig
%postun -n org.tizen.ode
/sbin/ldconfig

%define odeapp_home %{TZ_SYS_RO_APP}/org.tizen.ode

%files -n org.tizen.ode
%defattr(-,root,root,-)
%{odeapp_home}/bin/*
%{TZ_SYS_RO_PACKAGES}/org.tizen.ode.xml

## ZONE Setup Wizard Package #################################################
%package -n org.tizen.zone-setup-wizard
Summary: Tizen ZONE Setup wizard Interface
Group: Security/Other
BuildRequires: pkgconfig(efl-extension)
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(capi-appfw-application)
BuildRequires: pkgconfig(evas)
Requires: libdpm = %{version}-%{release}

%description -n org.tizen.zone-setup-wizard
Tizen ZONE setup wizard interface for zone

%post -n org.tizen.zone-setup-wizard
%{_sbindir}/ldconfig
%postun -n org.tizen.zone-setup-wizard
%{_sbindir}/ldconfig

%define setup_home %{TZ_SYS_RO_APP}/org.tizen.zone-setup-wizard

mkdir -p %{setup_home}/data

%files -n org.tizen.zone-setup-wizard
%defattr(-,root,root,-)
%manifest tools/zone-setup-wizard/org.tizen.zone-setup-wizard.manifest
%{setup_home}/bin/*
%{setup_home}/res/*
%{setup_home}/res/data/.sample-BundleManifest.xml
%{setup_home}/data
%{TZ_SYS_RO_PACKAGES}/org.tizen.zone-setup-wizard.xml

## PAM Plugin Package #######################################################
%package -n dpm-pam-zone
Summary: PAM Plugin for zone policy in device policy manager
Group: Development/Libraries
Requires: systemd

%description -n dpm-pam-zone
PAM Plugin for zone policy in device policy manager and CLI tool

%files -n dpm-pam-zone
%defattr(600,root,root,700)
%attr(700,root,root) %{_libdir}/security/pam_*.so
%attr(700,root,root) %{_sbindir}/nsattach
%attr(700,root,root) %{_sbindir}/activate-zone
%config /etc/pam.d/*

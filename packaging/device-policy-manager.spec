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
%attr(700,root,root) %dir %{TZ_SYS_ETC}/dpm/container
%attr(700,root,root) %config %{TZ_SYS_ETC}/dpm/container/owner.xml

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
	 -DCONTAINER_INSTALL_DIR=%{TZ_SYS_ETC}/dpm/container \
	 -DPAMD_INSTALL_DIR=/etc/pam.d \
     -DRUN_INSTALL_DIR=%{TZ_SYS_RUN} \

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

## PAM Package ##############################################################
%package -n dpm-pam-unshare
Summary: PAM for container policy in device policy manager
Group: Development/Libraries
Requires: systemd

%description -n dpm-pam-unshare
PAM for container policy in device policy manager and CLI tool

%files -n dpm-pam-unshare
%defattr(600,root,root,700)
%attr(700,root,root) %{_libdir}/security/pam_*.so
%attr(700,root,root) %{_sbindir}/nsattach
%config /etc/pam.d/*
